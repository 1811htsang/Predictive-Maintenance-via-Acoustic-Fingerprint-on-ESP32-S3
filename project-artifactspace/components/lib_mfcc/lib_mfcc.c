/**
 * @file lib_mfcc.c
 * @author Shang Huang
 * @brief MFCC (Mel-Frequency Cepstral Coefficients) library implementation file.
 * @version 0.1
 * @date 2026-06-06
 * @copyright Copyright (c) 2026
 */
#include "lib_mfcc.h"
#include "dsps_wind_hann.h"

esp_err_t mfcc_init(mfcc_config_t *cfg, int n_fft, int num_filters, int num_coeffs);

void mfcc_stage_preemphasis(float* data, int length, float alpha) {
  for (int i = length - 1; i > 0; i--) {
    data[i] = data[i] - alpha * data[i - 1];
  }
  data[0] = data[0]; // Không thay đổi phần tử đầu tiên
}

void mfcc_stage_frame_blocking(float* data, int length, int frame_size, int hop_size, float* output_frames, int* num_frames) {
  int frame_count = 0;
  for (int start = 0; start + frame_size <= length; start += hop_size) {
    for (int i = 0; i < frame_size; i++) {
      output_frames[frame_count * frame_size + i] = data[start + i];
    }
    frame_count++;
  }
  *num_frames = frame_count;
}

void mfcc_stage_windowing(float* frame_data, int frame_size, float* hann_window) {
  dsps_wind_hann_f32(hann_window, frame_size);
  for (int i = 0; i < frame_size; i++) {
    frame_data[i] *= hann_window[i];
  }
}

void mfcc_stage_fft(mfcc_config_t *cfg, float* time_data) {
  // Gọi init FFT nếu chưa được khởi tạo
  dsps_fft2r_init_fc32(cfg->fft_buffer, cfg->n_fft * 2); // Kích thước bộ đệm FFT là 2*n_fft do complex interleaved
  // Copy dữ liệu thời gian vào bộ đệm FFT (complex interleaved)
  for (int i = 0; i < cfg->n_fft; i++) {
    cfg->fft_buffer[2 * i] = time_data[i];     // Phần thực
    cfg->fft_buffer[2 * i + 1] = 0.0f;         // Phần ảo
  }
  // Thực hiện FFT sử dụng dsps_fft2r_f32
  dsps_fft2r_fc32_ae32(cfg->fft_buffer, cfg->n_fft);
}

void mfcc_compute_mel_log_energy(mfcc_config_t *cfg, float* mel_energies);

void mfcc_final_dct(mfcc_config_t *cfg, float* mel_log_energies, float* output_mfcc);