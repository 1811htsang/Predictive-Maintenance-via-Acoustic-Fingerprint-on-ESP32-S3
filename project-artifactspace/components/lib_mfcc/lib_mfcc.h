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
      float *fft_buffer;  // Bộ đệm trung gian cho FFT (complex interleaved)
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
  void mfcc_pre_emphasis(float* data, int length, float alpha);

  /**
   * @brief Áp dụng cửa sổ Hann và thực hiện FFT
   * Sử dụng dsps_fft2r_f32 tối ưu hóa bởi tập lệnh PIE của S3
   */
  void mfcc_process_fft(mfcc_config_t *cfg, float* time_data);

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