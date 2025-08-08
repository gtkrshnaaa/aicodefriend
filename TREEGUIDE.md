Expected Tree

-----

### Struktur File Proyek

Ini adalah gambaran `tree` yang bisa kita gunakan untuk proyek **AICODEFRIEND**. Strukturnya memisahkan dengan jelas antara antarmuka (UI), logika inti (core), komunikasi API, dan utilitas pendukung.

```
aicodefriend/
├── build/              # Folder untuk hasil kompilasi (dibuat oleh Makefile)
├── src/                # Semua kode sumber kita ada di sini
│   ├── main.c            # Titik masuk utama aplikasi
│   │
│   ├── core/             # Logika inti aplikasi
│   │   ├── app.h
│   │   ├── app.c           # Manajemen state aplikasi & penghubung utama
│   │   ├── prompt.h
│   │   ├── prompt.c        # Khusus untuk membangun JSON prompt
│   │   └── conversation.h
│   │   └── conversation.c  # Mengelola chat_history
│   │
│   ├── ui/               # Semua kode terkait GTK & antarmuka
│   │   ├── window.h
│   │   ├── window.c        # Pengaturan window utama
│   │   ├── chat_view.h
│   │   ├── chat_view.c     # Widget khusus untuk menampilkan chat (teks & kode)
│   │   └── settings.h
│   │   └── settings.c      # Dialog untuk memasukkan API Key
│   │
│   ├── api/              # Komunikasi dengan Gemini API
│   │   ├── gemini_client.h
│   │   ├── gemini_client.c # Mengirim request & menerima response (pakai libcurl)
│   │   └── response_parser.h
│   │   └── response_parser.c # Mem-parsing JSON & memisahkan teks dan kode
│   │
│   └── utils/            # Fungsi-fungsi bantuan umum
│       ├── string_utils.h
│       ├── string_utils.c  # Helper untuk manipulasi string
│       └── config.h
│       └── config.c      # Membaca/menyimpan API key dari/ke file
│
├── Makefile            # Perintah untuk build proyek (misal: make, make clean)
└── README.md           # Dokumentasi proyek (yang sudah kamu buat)
```

-----

### Penjelasan Setiap Folder

  * **`src/`**: Ini adalah rumah bagi semua kode `.c` dan `.h` yang akan kita tulis.
  * **`src/core/`**: Jantung dari aplikasi. Di sinilah kita menempatkan logika yang tidak berhubungan langsung dengan tampilan.
      * `app`: Sebagai dirigen yang menghubungkan UI events dengan fungsi-fungsi di `api` dan `core`.
      * `prompt`: Tugasnya satu, yaitu membuat struktur JSON lengkap yang akan dikirim ke Gemini sesuai rancanganmu.
      * `conversation`: Fokus mengelola riwayat percakapan, menambah pesan baru, dan menyiapkannya untuk `prompt`.
  * **`src/ui/`**: Semua yang dilihat dan diinteraksikan oleh pengguna.
      * `window`: Menginisialisasi window utama aplikasi.
      * `chat_view`: Ini komponen spesial. Dia akan pintar membedakan mana yang harus ditampilkan di `GtkTextView` (dengan Pango) dan mana yang di `GtkSourceView`.
      * `settings`: Sebuah dialog sederhana untuk pengguna memasukkan dan menyimpan API Key mereka.
  * **`src/api/`**: Jembatan antara aplikasi kita dengan dunia luar (Gemini API).
      * `gemini_client`: Tugasnya spesifik, yaitu melakukan koneksi HTTP, mengirim JSON, dan menerima balasan mentah.
      * `response_parser`: Setelah `gemini_client` dapat balasan, file ini yang akan "membongkar" JSON tersebut dan memisahkan antara teks biasa dan blok kode.
  * **`src/utils/`**: Perkakas kecil yang bisa dipakai di mana saja.
      * `string_utils`: Mungkin kita butuh fungsi untuk menggabungkan string atau mencari sesuatu, kita letakkan di sini.
      * `config`: Untuk menyimpan dan membaca Gemini API Key dari sebuah file di komputer pengguna, agar tidak perlu dimasukkan setiap kali aplikasi dibuka.

-----

### Peran File Header (`.h`)

Ini bagian penting yang sering bikin bingung. Bayangkan seperti ini:

  * **File `.h` (Header):** Ini adalah **daftar menu** atau "daftar isi". Isinya hanya deklarasi fungsi dan `struct`. File lain yang ingin menggunakan fungsi dari `app.c`, cukup membaca `app.h` untuk tahu fungsi apa saja yang tersedia.
  * **File `.c` (Source):** Ini adalah **dapurnya**. Di sinilah semua resep (kode implementasi) dari fungsi yang ada di daftar menu (`.h`) benar-benar ditulis.

Jadi, alurnya: `app.c` butuh fungsi dari `gemini_client.c`. Maka, di dalam `app.c`, kita cukup menulis `#include "api/gemini_client.h"`. Dengan begitu, `app.c` "tahu" fungsi apa saja yang bisa ia panggil dari modul `gemini_client`.

