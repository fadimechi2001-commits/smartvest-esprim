import numpy as np
np.random.seed(42)

def lms_filter(d, x, n_taps=16, mu=0.005):
    """Filtre LMS : d = ECG bruité, x = référence IMU."""
    w = np.zeros(n_taps)
    y_out = np.zeros(len(d))
    for n in range(n_taps, len(d)):
        x_vec = x[n - n_taps:n][::-1]
        y = np.dot(w, x_vec)
        e = d[n] - y
        w = w + 2 * mu * e * x_vec
        y_out[n] = e
    return y_out

if __name__ == "__main__":
    fs = 250
    t = np.arange(0, 10, 1 / fs)
    ecg = np.sin(2 * np.pi * 1.2 * t) ** 15
    motion = np.sin(2 * np.pi * 3 * t) * np.random.randn(len(t)) * 0.3
    noisy = ecg + motion
    clean = lms_filter(noisy, motion)
    snr = lambda s, ref: 10 * np.log10(np.sum(ref**2) / np.sum((s - ref)**2 + 1e-12))
    print("SNR avant :", round(snr(noisy, ecg), 2), "dB")
    print("SNR après :", round(snr(clean, ecg), 2), "dB")