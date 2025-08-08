#ifndef AICODEFRIEND_WINDOW_H
#define AICODEFRIEND_WINDOW_H

#include <adwaita.h>

// DIPERBAIKI: Forward declaration ini sekarang konsisten dengan app.h
typedef struct _AICodeFriendApp AICodeFriendApp;

GtkWidget *window_new(AICodeFriendApp *app);

#endif //AICODEFRIEND_WINDOW_H