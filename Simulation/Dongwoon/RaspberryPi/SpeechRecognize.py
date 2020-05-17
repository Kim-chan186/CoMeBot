#!/usr/bin/env python

# Copyright 2019 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""Google Cloud Speech API sample application using the streaming API.

NOTE: This module requires the dependencies `pyaudio` and `termcolor`.
To install using pip:

    pip install pyaudio
    pip install termcolor

Example usage:
    python transcribe_streaming_infinite.py
"""
# IMPORT for STT_GoogleAPI
import time
import re
import sys
from google.cloud import speech
import pyaudio
from six.moves import queue
# IMPORT for TCP & Mutex
import socket
from threading import Thread
import threading
import enum

# Audio recording parameters
STREAMING_LIMIT = 240000  # 4 minutes
SAMPLE_RATE = 16000
CHUNK_SIZE = int(SAMPLE_RATE / 10)  # 100ms
# TCP parameters
HOST = '192.168.0.33' 
PORT = 8585
# Terminal Console Print Color
RED = '\033[0;31m'
GREEN = '\033[0;32m'
YELLOW = '\033[0;33m'
# Mutex Variables
# condition_recv = threading.Condition()
condition_send = threading.Condition()
# Comebot Variables

stt_data = ""
stt_mode = 0
Comebot_Calling = ["코미봇"]
Compliment_Action = ["고마워", "고맙다"]
Compliment_Beauty = ["이쁘네", "귀엽네"]
Disappoint = ["실망이야"]
Sad = ["속상해", "서운해"]
Bored = ["심심해", "지루해"]
Sorry = ["미안해"]
Bad_Word = ["바보", "멍청이"]
stt_dict = {0 : Comebot_Calling, 
            1 : Compliment_Action,
            2 : Compliment_Beauty,
            3 : Disappoint,
            4 : Sad,
            5 : Bored,
            6 : Sorry,
            7 : Bad_Word
            }
     

def sendMsg(sock,cv_send):
    global stt_mode
    sock.send("stt".encode())
    while True:
        with cv_send:
            cv_send.acquire()
            cv_send.wait()
            cv_send.release()
            sock.send(str(stt_mode).encode())


                
"""Return Current Time in MS."""
def get_current_time():
    return int(round(time.time() * 1000))

    """Opens a recording stream as a generator yielding the audio chunks."""
class ResumableMicrophoneStream:
    def __init__(self, rate, chunk_size):
        self._rate = rate
        self.chunk_size = chunk_size
        self._num_channels = 1
        self._buff = queue.Queue()
        self.closed = True
        self.start_time = get_current_time()
        self.restart_counter = 0
        self.audio_input = []
        self.last_audio_input = []
        self.result_end_time = 0
        self.is_final_end_time = 0
        self.final_request_end_time = 0
        self.bridging_offset = 0
        self.last_transcript_was_final = False
        self.new_stream = True
        self._audio_interface = pyaudio.PyAudio()
        self._audio_stream = self._audio_interface.open(
            format=pyaudio.paInt16,
            channels=self._num_channels,
            rate=self._rate,
            input=True,
            frames_per_buffer=self.chunk_size,
            # Run the audio stream asynchronously to fill the buffer object.
            # This is necessary so that the input device's buffer doesn't
            # overflow while the calling thread makes network requests, etc.
            stream_callback=self._fill_buffer,
        )

    def __enter__(self):

        self.closed = False
        return self

    def __exit__(self, type, value, traceback):

        self._audio_stream.stop_stream()
        self._audio_stream.close()
        self.closed = True
        # Signal the generator to terminate so that the client's
        # streaming_recognize method will not block the process termination.
        self._buff.put(None)
        self._audio_interface.terminate()

    def _fill_buffer(self, in_data, *args, **kwargs):
        """Continuously collect data from the audio stream, into the buffer."""

        self._buff.put(in_data)
        return None, pyaudio.paContinue

    def generator(self):
        """Stream Audio from microphone to API and to local buffer"""

        while not self.closed:
            data = []

            if self.new_stream and self.last_audio_input:

                chunk_time = STREAMING_LIMIT / len(self.last_audio_input)

                if chunk_time != 0:

                    if self.bridging_offset < 0:
                        self.bridging_offset = 0

                    if self.bridging_offset > self.final_request_end_time:
                        self.bridging_offset = self.final_request_end_time

                    chunks_from_ms = round((self.final_request_end_time -
                                            self.bridging_offset) / chunk_time)

                    self.bridging_offset = (round((
                        len(self.last_audio_input) - chunks_from_ms)
                                                  * chunk_time))

                    for i in range(chunks_from_ms, len(self.last_audio_input)):
                        data.append(self.last_audio_input[i])

                self.new_stream = False

            # Use a blocking get() to ensure there's at least one chunk of
            # data, and stop iteration if the chunk is None, indicating the
            # end of the audio stream.
            chunk = self._buff.get()
            self.audio_input.append(chunk)

            if chunk is None:
                return
            data.append(chunk)
            # Now consume whatever other data's still buffered.
            while True:
                try:
                    chunk = self._buff.get(block=False)

                    if chunk is None:
                        return
                    data.append(chunk)
                    self.audio_input.append(chunk)

                except queue.Empty:
                    break

            yield b''.join(data)


def listen_print_loop(responses, stream):
    """Iterates through server responses and prints them.

    The responses passed is a generator that will block until a response
    is provided by the server.

    Each response may contain multiple results, and each result may contain
    multiple alternatives; for details, see https://goo.gl/tjCPAU.  Here we
    print only the transcription for the top alternative of the top result.

    In this case, responses are provided for interim results as well. If the
    response is an interim one, print a line feed at the end of it, to allow
    the next result to overwrite it, until the response is a final one. For the
    final one, print a newline to preserve the finalized transcription.
    """

    for response in responses:

        if get_current_time() - stream.start_time > STREAMING_LIMIT:
            stream.start_time = get_current_time()
            break

        if not response.results:
            continue
        # print("response: ",response)
        result = response.results[0]
        # print("result: ",result)
        if not result.alternatives:
            continue

        transcript = result.alternatives[0].transcript
        #print(type(transcript))
        result_seconds = 0
        result_nanos = 0

        if result.result_end_time.seconds:
            result_seconds = result.result_end_time.seconds

        if result.result_end_time.nanos:
            result_nanos = result.result_end_time.nanos

        stream.result_end_time = int((result_seconds * 1000)
                                     + (result_nanos / 1000000))

        corrected_time = (stream.result_end_time - stream.bridging_offset
                          + (STREAMING_LIMIT * stream.restart_counter))
        # Display interim results, but with a carriage returnAK at the end of the
        # line, so subsequent lines will overwrite them.

        if result.is_final:

            sys.stdout.write(GREEN)
            sys.stdout.write('\033[K')
            sys.stdout.write(str(corrected_time) + ': ' + transcript + '\n')

            stream.is_final_end_time = stream.result_end_time
            stream.last_transcript_was_final = True

            # Exit recognition if any of the transcribed phrases could be
            # one of our keywords.
            if re.search(r'\b(exit|quit)\b', transcript, re.I):
                sys.stdout.write(YELLOW)
                sys.stdout.write('Exiting...\n')
                stream.closed = True
                break
            return transcript
            
        else:
            sys.stdout.write(RED)
            sys.stdout.write('\033[K')
            sys.stdout.write(str(corrected_time) + ': ' + transcript + '\r')

            stream.last_transcript_was_final = False


def Comebot_Voice(cv_send):
    # Comebot Global Variable
    global stt_data # string
    global stt_mode # int
    global stt_dict # list
    pre_script = ""
    """start bidirectional streaming from microphone input to speech API"""
    # Initialize of STT_GoogleAPI
    client = speech.SpeechClient()
    boost = 20.0
    speech_contexts_element = {"phrases":["코미봇"] }
    speech_contexts = [speech_contexts_element]
    config = speech.types.RecognitionConfig(
        encoding=speech.enums.RecognitionConfig.AudioEncoding.LINEAR16,
        sample_rate_hertz=SAMPLE_RATE,
        speech_contexts= speech_contexts,
        language_code='ko-KR',
        #language_code='en-US',
        max_alternatives=1)
    streaming_config = speech.types.StreamingRecognitionConfig(
        config=config,
        # single_utterance = True,
        interim_results=True)
    mic_manager = ResumableMicrophoneStream(SAMPLE_RATE, CHUNK_SIZE)
    print(mic_manager.chunk_size)
    sys.stdout.write(YELLOW)
    sys.stdout.write('\nListening, say "Quit" or "Exit" to stop.\n\n')
    sys.stdout.write('End (ms)       Transcript Results/Status\n')
    sys.stdout.write('=====================================================\n')


    with mic_manager as stream:
        while not stream.closed:
            # Speech Initialization Setting
            sys.stdout.write(YELLOW)
            sys.stdout.write('\n' + str(
                STREAMING_LIMIT * stream.restart_counter) + ': NEW REQUEST\n')
            stream.audio_input = []
            audio_generator = stream.generator()
            requests = (speech.types.StreamingRecognizeRequest(
                audio_content=content)for content in audio_generator)

            responses = client.streaming_recognize(streaming_config,
                                                   requests)

            # Now, put the transcription responses to use.
            script = listen_print_loop(responses, stream)
            recogn_flag = 0
            if pre_script != script:
                print(script,":",type(script))
                script_words = script.split(' ')
                for word in script_words:
                    for stt_num,stt_list in stt_dict.items():
                        for stt in stt_list:
                            if stt == word:
                                stt_mode = stt_num
                                print(stt,":",stt_num)
                                if recogn_flag == 1 :
                                    print("send\n")
                                    # Unlock Mutex
                                    cv_send.acquire()
                                    cv_send.notifyAll()
                                    cv_send.release()
                                if stt_mode == 0:
                                    recogn_flag = 1
                                
            pre_script = script
            if stream.result_end_time > 0:
                stream.final_request_end_time = stream.is_final_end_time
            stream.result_end_time = 0
            stream.last_audio_input = []
            stream.last_audio_input = stream.audio_input
            stream.audio_input = []
            stream.restart_counter = stream.restart_counter + 1
            if not stream.last_transcript_was_final:
                sys.stdout.write('\n')
            stream.new_stream = True

def Compact_Thread():
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
		sock.connect((HOST, PORT))
        
		thread_STT_Google = Thread(target=Comebot_Voice, args=(condition_send,))
		thread_send = Thread(target=sendMsg, args=(sock,condition_send,))
		# thread_stt = Thread(target=STTThread, args=(condition_send, condition_recv))
        
		thread_STT_Google.daemon = True
		thread_send.daemon = True
		# thread_stt.daemon = True
        
		thread_STT_Google.start()
		thread_send.start()
		# thread_stt.start()
        
		while(1):
			pass

if __name__ == '__main__':
    Compact_Thread()
