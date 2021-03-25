import sys
import os

import numpy as np

from tqdm import tqdm

from inferer import Inferer

def main(args):
    tacotron2_path_model_1 = 'tacotron2_published.pt'
    waveglow_path_model_1 = 'waveglow_published.pt'
    tacotron2_path_model_2 = 'tacotron2_16kHz.pt'
    waveglow_path_model_2 = 'waveglow_16kHz.pt'

    # if all models aren't at the same sample rate, there will be problems, could fix with a conversion ratio
    sample_rate_model_1 = 22050
    sample_rate_model_2 = 16000

    inferer1 = Inferer(tacotron2_path_model_1, waveglow_path_model_1, sample_rate_model_1)
    inferer2 = Inferer(tacotron2_path_model_2, waveglow_path_model_2, sample_rate_model_2)

    phrase1 = ['Hi there Angela.', inferer1]
    phrase2 = ['Oh, hi Jane.', inferer2]
    phrase3 = ['How are you today?', inferer1]
    phrase4 = ['Just fine thanks, yourself?', inferer2]
    phrase5 = ['Well enough, cheers.', inferer1]

    phrases = [phrase1, phrase2, phrase3, phrase4, phrase5]

    running_audio_numpy = np.zeros(0, dtype='float32')
    for phrase in tqdm(phrases):
        phrase_audio = phrase[1].infer(phrase[0])
        running_audio_numpy = phrase[1].stitch_audio(running_audio_numpy, phrase_audio)

    inferer2.write_wav('stitched_multi.wav', running_audio_numpy)


if __name__ == '__main__':
    main(sys.argv[1:])
