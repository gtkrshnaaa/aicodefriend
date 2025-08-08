Written in Bahasa Indonesia

-----

# **AICODEFRIEND - An AI coding companion that helping u like a parter in focus.**

Proyek ini bertujuan untuk menciptakan sebuah aplikasi desktop *open-source* yang ringan dan cepat, berfungsi sebagai *AI coding companion* yang sangat personal. Dibangun menggunakan bahasa pemrograman **C** dan **GTK** untuk antarmuka pengguna, aplikasi ini memanfaatkan **Gemini API** sebagai mesin kecerdasannya. Fokus utama proyek ini adalah menyediakan pengalaman coding yang terintegrasi, di mana balasan dari AI diproses secara terstruktur dan ditampilkan dengan indah, sesuai dengan preferensi unik setiap penggunanya.

### **Latar Belakang & Tujuan**

#### **Latar Belakang**

Banyak alat bantu AI saat ini sering kali terbatas pada antarmuka web atau IDE, yang sering kali mengorbankan performa atau fleksibilitas. Proyek ini lahir dari visi untuk menciptakan alat yang benar-benar milik pengembang, sebuah aplikasi *native* yang efisien, dapat diandalkan, dan yang paling penting, bisa menyesuaikan diri dengan gaya dan kebutuhan spesifik pengguna, tanpa harus terikat pada platform lain.

#### **Tujuan Proyek**

1.  **Performa Optimal:** Memanfaatkan C dan GTK untuk menciptakan aplikasi yang ringan, cepat, dan minim konsumsi sumber daya.
2.  **Integrasi Mendalam:** Menghubungkan Gemini API dengan logika aplikasi C untuk memproses respons secara cerdas.
3.  **Personalisasi Tingkat Tinggi:** Membangun struktur prompt yang fleksibel dan terperinci, memungkinkan pengguna mendefinisikan persona AI dan gaya coding mereka sendiri.
4.  **API Key Mandiri:** Aplikasi ini memerlukan **Gemini API Key** yang disediakan secara mandiri oleh pengguna, dengan antarmuka di dalam aplikasi untuk memasukkannya.
5.  **Output Terstruktur:** Menyediakan mekanisme untuk memisahkan teks dan blok kode secara sistematis, menggunakan Pango Markup dan tag khusus untuk rendering yang presisi.

-----

### **Konsep Arsitektur**

Secara konseptual, aplikasi ini terdiri dari tiga komponen utama yang bekerja dalam satu alur terpadu.

1.  **Antarmuka Pengguna (UI)**
      * Dibangun dengan GTK.
      * Menggunakan **`GTKTextView`** untuk menampilkan teks biasa, yang akan memproses format **Pango Markup** dari respons AI.
      * Menggunakan **`GTKSourceView`** untuk menampilkan blok kode secara khusus, dengan fitur *syntax highlighting* yang profesional.
2.  **Logika Aplikasi (Core Logic)**
      * Ditulis dalam bahasa C.
      * Bertanggung jawab untuk:
          * Membangun dan mengirim permintaan JSON ke Gemini API.
          * Menerima dan mem-parsing respons JSON.
          * Memisahkan teks biasa dan blok kode berdasarkan tag khusus (`<code-block lang=\"...\">...</code-block>`).
          * Menampilkan output yang sudah dipisahkan ke widget GTK yang sesuai.
3.  **Mesin AI (AI Engine)**
      * Menggunakan Gemini API sebagai otak di balik percakapan.
      * Menerima prompt yang sangat terstruktur dalam format JSON.

-----

### **Struktur Prompt JSON (AI Communication Protocol)**

Ini adalah protokol komunikasi yang akan digunakan aplikasi untuk berinteraksi dengan Gemini API. Setiap bagian memiliki peran penting dalam membentuk respons yang cerdas dan sesuai.

````json
{
  "system_context": {
    "role": "system",
    "content": "This is an AI Coding Companion. AI model core task is to assist the user by providing structured responses. All normal text must be formatted using Pango Markup. All code blocks must be enclosed within the custom tags <code-block lang=\"...\">...</code-block>. It is strictly forbidden to use triple backtick markdown (```)."
  },
  "ai_persona": {
    "role": "model",
    "content": "Ai is an assistant with a calm, friendly, and wise personality, acting as a thoughtful companion."
  },
  "user_persona": {
    "role": "user",
    "content": "The user is a Software Engineer experienced in C, Java, Go, PHP, JS, and Python. The user likes writing clean and tidy code. For web styling, the user prefers using Tailwind with a soft, 'milky' color scheme. The user also tends to think conceptually and is a visionary."
  },
  "chat_history": [
    {
      "role": "user",
      "content": "I'm thinking about how to create a C function to read a JSON file."
    },
    {
      "role": "model",
      "content": "That's an interesting idea. C can be a bit tricky with JSON parsing, but there are some great libraries to help. We could use libraries like <span weight=\"bold\">json-c</span> or <span weight=\"bold\">jansson</span>. For example, using a library is often simpler than parsing it from scratch.\n\n<code-block lang=\"c\">\n// Here's a basic idea for loading a JSON file with a library.\n#include <stdio.h>\n#include <stdlib.h>\n#include <json-c/json.h>\n\nint main() {\n    printf(\"Example JSON parsing\\n\");\n    // ... code to load file ...\n    return 0;\n}\n</code-block>\n\nMaybe you'd like to explore one of those first?"
    }
  ],
  "current_user_input": {
    "role": "user",
    "content": "I want to try using `jansson`, it seems easier."
  }
}
````

-----

### **Penjelasan Detail Konsep**

#### **Pondasi Mutlak: `system_context`**

Ini adalah fondasi dari seluruh sistem. Bagian ini berisi perintah-perintah yang bersifat absolut dan tidak boleh diubah oleh AI. `system_context` bertugas memberikan identitas, peran, dan yang terpenting, aturan format output yang wajib dipatuhi.

````json
{
  "system_context": {
    "role": "system",
    "content": "This is an AI Coding Companion. AI model core task is to assist the user by providing structured responses. All normal text must be formatted using Pango Markup. All code blocks must be enclosed within the custom tags <code-block lang=\"...\">...</code-block>. It is strictly forbidden to use triple backtick markdown (```)."
  }
}
````

#### **Karakter & Jiwa: `ai_persona`**

Blok ini mendefinisikan kepribadian dan gaya komunikasi AI. Dengan adanya `ai_persona`, model tidak hanya sekadar memberikan jawaban, tetapi juga berinteraksi dengan cara yang spesifik—sebagai asisten yang tenang, bijaksana, dan ramah.

```json
{
  "ai_persona": {
    "role": "model",
    "content": "Ai is an assistant with a calm, friendly, and wise personality, acting as a thoughtful companion."
  }
}
```

#### **Pencipta & Pemikir: `user_persona`**

Bagian ini adalah kunci personalisasi. `user_persona` memberikan AI informasi tentang siapa pengguna, latar belakang teknis, preferensi gaya coding, hingga cara berpikirnya. Ini memungkinkan AI memberikan jawaban yang benar-benar relevan dan selaras dengan cara kerja Kiann.

```json
{
  "user_persona": {
    "role": "user",
    "content": "The user is a Software Engineer experienced in C, Java, Go, PHP, JS, and Python. The user likes writing clean and tidy code. For web styling, the user prefers using Tailwind with a soft, 'milky' color scheme. The user also tends to think conceptually and is a visionary."
  }
}
```

#### **Ingatan & Alur Cerita: `chat_history`**

Ini adalah rekaman percakapan dari awal hingga saat ini. `chat_history` berfungsi sebagai ingatan AI, yang memberikan konteks penuh dari diskusi sebelumnya. Tanpa ini, AI akan bingung dan tidak bisa melanjutkan alur cerita dengan benar.

```json
{
  "chat_history": [
    {
      "role": "user",
      "content": "I'm thinking about how to create a C function to read a JSON file."
    },
    {
      "role": "model",
      "content": "That's an interesting idea. C can be a bit tricky with JSON parsing, but there are some great libraries to help. We could use libraries like <span weight=\"bold\">json-c</span> or <span weight=\"bold\">jansson</span>. For example, using a library is often simpler than parsing it from scratch.\n\n<code-block lang=\"c\">\n// Here's a basic idea for loading a JSON file with a library.\n#include <stdio.h>\n#include <stdlib.h>\n#include <json-c/json.h>\n\nint main() {\n    printf(\"Example JSON parsing\\n\");\n    // ... code to load file ...\n    return 0;\n}\n</code-block>\n\nMaybe you'd like to explore one of those first?"
    }
  ]
}
```

#### **Pemicu & Babak Baru: `current_user_input`**

Ini adalah input terbaru dari Kiann. `current_user_input` adalah pemicu yang menggerakkan seluruh proses, tempat di mana AI akan membaca permintaan dan mulai menyusun respons yang baru.

```json
{
  "current_user_input": {
    "role": "user",
    "content": "I want to try using `jansson`, it seems easier."
  }
}
```

-----

### **Kesimpulan**

Proyek ini bukan hanya tentang menciptakan aplikasi chat biasa, melainkan sebuah manifestasi dari keinginan untuk membangun sebuah *AI coding companion* yang benar-benar personal dan efisien. Dengan memanfaatkan kekuatan C dan GTK untuk performa, serta fleksibilitas Gemini API untuk kecerdasan, proyek ini menawarkan pendekatan segar dalam kolaborasi antara manusia dan AI. Ini adalah sebuah alat yang dibangun dari awal, untuk para visioner, agar bisa berkolaborasi dengan cara yang paling alami dan intuitif.
