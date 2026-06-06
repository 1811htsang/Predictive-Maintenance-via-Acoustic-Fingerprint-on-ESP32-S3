/**
 * @file lib_mfcc.h
 * @author Shang Huang
 * @brief MFCC (Mel-Frequency Cepstral Coefficients) library header file.
 * @version 0.1
 * @date 2026-05-28
 * @copyright GNU General Public License v3.0
 */
#ifndef __LIB_MFCC_H__
  #define __LIB_MFCC_H__

  #include <stdio.h>
  #include <stdlib.h>
  #include "esp_err.h"
  #include "dsps_fft2r.h" // Thư viện gốc của Espressif

  /**
   * @brief Cấu trúc quản lý trạng thái MFCC để tránh tính toán lại hằng số
   */
  typedef struct {
    int n_fft;          // Số điểm FFT (ví dụ 1024)
    int num_filters;    // Số bộ lọc Mel (ví dụ 26 hoặc 40)
    int num_coeffs;     // Số hệ số MFCC đầu ra (ví dụ 13)
    float *hann_window; // Mảng chứa hằng số cửa sổ Hann
    float *mel_weights; // Ma trận trọng số bộ lọc Mel (nên tính sẵn trên PC)
    float *fft_buffer;  // Bộ đệm trung gian cho FFT (complex interleaved) chẵn lẻ xen kẽ
  } mfcc_config_t;

  /**
   * @brief Khởi tạo các tài nguyên cần thiết cho MFCC (chạy 1 lần lúc boot)
   * @return esp_err_t 
   */
  esp_err_t mfcc_init(mfcc_config_t *cfg, int n_fft, int num_filters, int num_coeffs);

  /**
   * @brief Tiền nhấn (Pre-emphasis)
   * Thực hiện: y[n] = x[n] - alpha * x[n-1]
   */
  void mfcc_stage_preemphasis(float* data, int length, float alpha);

  /**
   * @brief Chia khung (Framing) - cắt tín hiệu thành các khung nhỏ, thường có chồng lấn (overlap)
   * @param data Dữ liệu đầu vào (dạng mảng float)
   * @param length Độ dài của dữ liệu đầu vào
   * @param frame_size Kích thước khung (ví dụ 400 mẫu cho 25ms ở 16kHz)
   * @param hop_size Kích thước bước nhảy (ví dụ 160 mẫu cho 10ms ở 16kHz)
   * @param output_frames Mảng đầu ra chứa các khung đã được cắt (kích thước = num_frames * frame_size)
   * @param num_frames Số lượng khung được tạo ra (được trả về qua con trỏ)
   */
  void mfcc_stage_frame_blocking(float* data, int length, int frame_size, int hop_size, float* output_frames, int* num_frames);

  /**
   * @brief Áp dụng cửa sổ Hann cho mỗi khung
   * @param frame_data Dữ liệu khung đầu vào (kích thước = frame_size)
   * @param frame_size Kích thước của khung
   * @param hann_window Mảng chứa hằng số cửa sổ Hann (kích thước = frame_size)
   */
  void mfcc_stage_windowing(float* frame_data, int frame_size, float* hann_window);

  /**
   * @brief Áp dụng cửa sổ Hann và thực hiện FFT
   * Sử dụng dsps_fft2r_f32 tối ưu hóa bởi tập lệnh PIE của S3
   */
  void mfcc_stage_fft(mfcc_config_t *cfg, float* time_data);

  /**
   * @brief Tính toán năng lượng tại các băng tần Mel và lấy LOG
   * @param mel_energies Mảng lưu kết quả log-energy (kích thước = num_filters)
   */
  void mfcc_compute_mel_log_energy(mfcc_config_t *cfg, float* mel_energies);

  /**
   * @brief Biến đổi Cosine rời rạc (DCT) để ra hệ số cuối cùng
   * Sử dụng dsps_dct_f32
   */
  void mfcc_final_dct(mfcc_config_t *cfg, float* mel_log_energies, float* output_mfcc);

#endif