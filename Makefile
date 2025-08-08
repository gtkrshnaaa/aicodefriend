# === Target & Compiler ===
TARGET := aicodefriend
CC := gcc

# === Direktori ===
SRC_DIR := src
CORE_DIR := $(SRC_DIR)/core
UI_DIR := $(SRC_DIR)/ui
API_DIR := $(SRC_DIR)/api
UTILS_DIR := $(SRC_DIR)/utils

# === Flags & Libraries ===
CFLAGS := $(shell pkg-config --cflags gtk4 libadwaita-1) -I$(SRC_DIR) -Wall
LIBS := $(shell pkg-config --libs gtk4 libadwaita-1)

# === File Source & Object ===
SOURCES := $(wildcard $(SRC_DIR)/*.c $(CORE_DIR)/*.c $(UI_DIR)/*.c)
OBJECTS := $(SOURCES:.c=.o)


# ==============================================================================
# ATURAN UTAMA
# ==============================================================================

# Target default: build aplikasi
all: $(TARGET)

# Aturan untuk membuat executable utama
$(TARGET): $(OBJECTS)
	@echo "Linking..."
	@$(CC) -o $@ $^ $(LIBS)
	@echo "✅ $(TARGET) siap dijalankan!"

# Aturan untuk mengkompilasi file .c menjadi .o
%.o: %.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Target untuk menjalankan siklus: compile -> run -> clean
run: $(TARGET)
	@echo "🚀 Running application..."
	@./$(TARGET)
	@echo "🧹 Cleaning up..."
	@$(MAKE) clean

# Target untuk membersihkan file hasil build
clean:
	@rm -f $(TARGET) $(OBJECTS)


# ==============================================================================
# ATURAN UTILITAS (Listing File)
# ==============================================================================

LISTING_DIR := z_listing
LISTING_FILE := $(LISTING_DIR)/listing.txt
ROOT := .

# Target untuk membuat listing semua file proyek
list:
	@mkdir -p $(LISTING_DIR)
	@echo "Generating file listing into $(LISTING_FILE)..."
	@> $(LISTING_FILE)
	@find $(ROOT) -type f \
		! -path "$(LISTING_FILE)" \
		! -path "./$(LISTING_DIR)/*" \
		! -path "./.git/*" \
		! -path "./.idea/*" \
		! -name "$(TARGET)" \
		! -name "*.o" \
	| while read file; do \
		echo "=== $$file ===" >> $(LISTING_FILE); \
		cat $$file >> $(LISTING_FILE); \
		echo "\n" >> $(LISTING_FILE); \
	done
	@echo "✅ File listing generated at $(LISTING_FILE)"

# Mendeklarasikan target yang bukan nama file
.PHONY: all clean list run