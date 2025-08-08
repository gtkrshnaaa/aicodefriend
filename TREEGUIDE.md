Expected Tree

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