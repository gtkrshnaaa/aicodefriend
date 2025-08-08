#ifndef AICODEFRIEND_WINDOW_H
#define AICODEFRIEND_WINDOW_H

#include <adwaita.h>

// Deklarasi fungsi untuk membuat GtkWidget (jendela utama) baru.
// Fungsi ini akan dipanggil oleh app.c saat aplikasi aktif.
GtkWidget *window_new(AdwApplication *app);

#endif //AICODEFRIEND_WINDOW_H