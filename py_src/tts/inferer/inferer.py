import sys
sys.path.append('../training/')
sys.path.append('../training/waveglow/')

import numpy as np
import matplotlib.pylab as plt
import torch

from hparams import create_hparams
from model import Tacotron2
from layers import TacotronSTFT, STFT
from audio_processing import griffin_lim
from train import load_model
from text import text_to_sequence
from denoiser import Denoiser

from scipy.io.wavfile import write as write_wav, read as read_wav

class Inferer:
    # Population parameters
    tacotron2 = None
    waveglow = None
    sample_rate = 0
    denoiser = None

    # Init method
    def __init__(self, tacotron2_path, waveglow_path, sample_rate):
        # create hparams
        hparams = create_hparams()
        hparams.sampling_rate = sample_rate

        # Load the tacotron2 model to the class
        model = load_model(hparams, use_cpu=True)
        model.load_state_dict(torch.load(tacotron2_path, map_location=torch.device('cpu'))['state_dict'])
        _ = model.cpu().eval()
        self.tacotron2 = model

        # Load the waveglow model to the class
        waveglow = torch.load(waveglow_path, map_location=torch.device('cpu'))['model']
        waveglow.cpu().eval()
        self.waveglow = waveglow

        # Load the denoiser to the class
        for k in waveglow.convinv:
            k.float()
        self.denoiser = Denoiser(waveglow, use_cpu=True)

        # set the sample rate
        self.sample_rate = sample_rate

    def write_wav(self, output_path, audio_numpy):
        write_wav(output_path, self.sample_rate, audio_numpy)

    def infer(self, text, output_path=None, all_inference_data=False):
        # Formulate sequence for given text
        sequence = np.array(text_to_sequence(text, ['english_cleaners']))[None, :]
        sequence = torch.autograd.Variable(
            torch.from_numpy(sequence)).cpu().long()

        # get mel outputs
        mel_outputs, mel_outputs_postnet, _, alignments = self.tacotron2.inference(sequence)

        # get audio, using waveglow model
        with torch.no_grad():
            audio = self.waveglow.infer(mel_outputs_postnet, sigma=0.666)

        # denoise the audio
        audio_denoised = self.denoiser(audio, strength=0.01)[:, 0]

        # get numpy representation
        denoised_audio_numpy = audio_denoised[0].data.cpu().numpy()

        # if specified, write to wav file
        if output_path != None:
            self.write_wav(output_path, denoised_audio_numpy)

        # return either just the denoised nump array, or all the inference data, in a dict
        if all_inference_data:
            all_data = {}
            all_data["audio_numpy"] = audio[0].data.cpu().numpy()
            all_data["denoised_audio_numpy"] = denoised_audio_numpy
            # adding the information needed for plotting mels and alignment visualisations
            all_data["mel_outputs_numpy"] = mel_outputs.float().data.cpu().numpy()[0]
            all_data["mel_outputs_postnet_numpy"] = mel_outputs_postnet.float().data.cpu().numpy()[0]
            all_data["alignments_numpy_T"] = alignments.float().data.cpu().numpy()[0].T

            return all_data
        else:
            return denoised_audio_numpy

    def plot_data(self, data, figsize=(16, 4)):
        fig, axes = plt.subplots(1, len(data), figsize=figsize)
        for i in range(len(data)):
            axes[i].imshow(data[i], aspect='auto', origin='bottom',
                           interpolation='none')

    def stitch_audio(self, audio_1_numpy, audio_2_numpy, interval_seconds=0.5):
        samples_in_interval = int(interval_seconds * self.sample_rate)
        interval_numpy = np.zeros(samples_in_interval, dtype='float32')

        stitched_audio = np.concatenate((audio_1_numpy, interval_numpy, audio_2_numpy), axis=0)

        return stitched_audio

    def stitch_audio_files(self, filepath_1, filepath_2, output_path=None):
        _, audio_1_numpy = read_wav(filepath_1)
        _, audio_2_numpy = read_wav(filepath_2)
        stitched_audio_numpy = self.stitch_audio(audio_1_numpy, audio_2_numpy)

        # write to wav file or return audio numpy
        if output_path != None:
            self.write_wav(output_path, stitched_audio_numpy)
        else:
            return audio_numpy
