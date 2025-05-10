#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_BOOKS 100
#define MAX_USERS 100
#define SHIFT_PASSWORD 20

// === Book ===
// Enum untuk status buku
enum BookStatus {
    KOSONG,
    TERSEDIA
};

// Struct buku
typedef struct {
    int id;
    char title[100];
    char author[100];
    enum BookStatus status;
} Book;

// Array buku
Book books[MAX_BOOKS];

// Total buku saat ini
int bookCount = 0;

// ID generator agar selalu unik
int nextId = 1;

// === Users ===
// Type User
enum TypeUser {
    ADMIN,
    MEMBER
};

// Struktur User
typedef struct {
    int id;
    char nama_lengkap[100];
    char password[100];
    enum TypeUser type;
} User;

// Array user
User users[MAX_USERS];

// Total user saat ini
int userCount = 0;

// ID generator untuk user
int nextUserId = 1;

/**
 * Fungsi untuk mengenkripsi password menggunakan Caesar cipher
 * Menggeser karakter alfabet sebanyak SHIFT_PASSWORD
 */
void encrypt(char *text) {
    for(int i = 0; text[i] != '\0'; i++){
        char ch = text[i];
        if(isupper(ch)){
            text[i] = ((ch - 'A' + SHIFT_PASSWORD) % 26) + 'A';
        } else if(islower(ch)){
            text[i] = ((ch - 'a' + SHIFT_PASSWORD) % 26) + 'a';
        }
    }
}

/**
 * Fungsi untuk mendekripsi password
 * Mengembalikan karakter ke posisi awal sebelum enkripsi
 */
void decrypt(char *text) {
    for(int i = 0; text[i] != '\0'; i++){
        char ch = text[i];
        if(isupper(ch)){
            text[i] = ((ch - 'A' + (26-SHIFT_PASSWORD)) % 26) + 'A';
        } else if(islower(ch)){
            text[i] = ((ch - 'a' + (26-SHIFT_PASSWORD)) % 26) + 'a';
        }
    }
}

/**
 * Fungsi untuk menambahkan buku ke array books
 * ID akan selalu unik dan tidak tergantung pada indeks array
 */
void tambahBuku() {
    if (bookCount >= MAX_BOOKS) {
        printf("Kapasitas penuh!\n");
        return;
    }

    Book book;
    bool statusValid = false;

    book.id = nextId++;

    printf("Masukkan judul buku: ");
    fgets(book.title, sizeof(book.title), stdin);
    book.title[strcspn(book.title, "\n")] = 0;

    printf("Masukkan author buku: ");
    fgets(book.author, sizeof(book.author), stdin);
    book.author[strcspn(book.author, "\n")] = 0;

    while (!statusValid) {
        int statusInput;
        printf("Masukkan status peminjaman buku\n");
        printf("0: Kosong\n1: Tersedia\nPilih salah satu angka di atas: ");
        if (scanf("%d", &statusInput) == 1 && (statusInput == 0 || statusInput == 1)) {
            book.status = statusInput;
            statusValid = true;
        } else {
            printf("Input tidak valid, coba lagi!\n");
            while (getchar() != '\n');
        }
    }

    while (getchar() != '\n');

    books[bookCount++] = book;
    printf("Buku berhasil ditambahkan!\n");
}

/**
 * Fungsi untuk menampilkan seluruh data buku yang ada
 */
void tampilBuku() {
    if (bookCount == 0) {
        printf("Tidak ada data buku.\n");
        return;
    }

    printf("=== Daftar Buku ===\n");
    for (int i = 0; i < bookCount; i++) {
        printf("====================================\n");
        printf("ID: %d\nJudul: %s\nAuthor: %s\nStatus: %s\n",
               books[i].id,
               books[i].title,
               books[i].author,
               books[i].status == TERSEDIA ? "Tersedia" : "Kosong");
    }
}

/**
 * Fungsi untuk mengubah data buku berdasarkan ID
 * Jika ID ditemukan, pengguna dapat memperbarui judul, author, dan status
 */
void updateBuku() {
    int id;
    bool statusValid = false;

    if (bookCount == 0) {
        printf("Tidak ada data buku.\n");
        return;
    }

    tampilBuku();
    printf("\nMasukan ID buku yang ingin anda ubah: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == id) {
            printf("Masukkan judul buku: ");
            fgets(books[i].title, sizeof(books[i].title), stdin);
            books[i].title[strcspn(books[i].title, "\n")] = 0;

            printf("Masukkan author buku: ");
            fgets(books[i].author, sizeof(books[i].author), stdin);
            books[i].author[strcspn(books[i].author, "\n")] = 0;

            while (!statusValid) {
                int statusInput;
                printf("Masukkan status peminjaman buku\n");
                printf("0: Kosong\n1: Tersedia\nPilih salah satu angka di atas: ");
                if (scanf("%d", &statusInput) == 1 && (statusInput == 0 || statusInput == 1)) {
                    books[i].status = statusInput;
                    statusValid = true;
                } else {
                    printf("Input tidak valid, coba lagi!\n");
                    while (getchar() != '\n');
                }
            }

            while (getchar() != '\n');
            printf("\nData berhasil dirubah\n");
            return;
        }
    }

    printf("\nData buku dengan ID %d tidak ditemukan\n", id);
}

/**
 * Fungsi untuk menghapus buku dari array berdasarkan ID
 * Buku akan dihapus dan array digeser ke kiri
 */
void hapusBuku() {
    int id;
    if (bookCount == 0) {
        printf("Tidak ada data buku.\n");
        return;
    }

    tampilBuku();
    printf("\nMasukan ID buku yang ingin anda hapus: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == id) {
            for (int j = i; j < bookCount - 1; j++) {
                books[j] = books[j + 1];
            }
            bookCount--;
            printf("Buku berhasil dihapus\n");
            return;
        }
    }

    printf("\nData buku dengan ID %d tidak ditemukan\n", id);
}

/**
 * Fungsi untuk registrasi member baru
 * Menambahkan user baru ke array users dengan tipe MEMBER
 */
void registerMember() {
    if (userCount >= MAX_USERS) {
        printf("Kapasitas pengguna penuh!\n");
        return;
    }

    User user;
    user.id = nextUserId++;
    user.type = MEMBER;

    printf("Masukkan nama lengkap: ");
    fgets(user.nama_lengkap, sizeof(user.nama_lengkap), stdin);
    user.nama_lengkap[strcspn(user.nama_lengkap, "\n")] = 0;

    printf("Masukkan password: ");
    fgets(user.password, sizeof(user.password), stdin);
    user.password[strcspn(user.password, "\n")] = 0;
    encrypt(user.password);

    users[userCount++] = user;
    printf("Registrasi berhasil!\n");
}

/**
 * Fungsi untuk login member
 * Mengembalikan indeks user jika login berhasil, -1 jika gagal
 */
int loginMember() {
    char nama[100];
    char password[100];

    printf("Masukkan nama lengkap: ");
    fgets(nama, sizeof(nama), stdin);
    nama[strcspn(nama, "\n")] = 0;

    printf("Masukkan password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;
    encrypt(password);

    for (int i = 0; i < userCount; i++) {
        if (users[i].type == MEMBER && strcmp(users[i].nama_lengkap, nama) == 0 && 
            strcmp(users[i].password, password) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * Fungsi untuk login admin
 * Mengembalikan indeks user jika login berhasil, -1 jika gagal
 */
int loginAdmin() {
    char nama[100];
    char password[100];

    printf("Masukkan nama lengkap: ");
    fgets(nama, sizeof(nama), stdin);
    nama[strcspn(nama, "\n")] = 0;

    printf("Masukkan password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;
    encrypt(password);

    for (int i = 0; i < userCount; i++) {
        if (users[i].type == ADMIN && strcmp(users[i].nama_lengkap, nama) == 0 && 
            strcmp(users[i].password, password) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * Fungsi untuk menampilkan semua user
 * Hanya dapat diakses oleh admin
 */
void tampilUsers() {
    if (userCount == 0) {
        printf("Tidak ada data pengguna.\n");
        return;
    }

    printf("=== Daftar Pengguna ===\n");
    for (int i = 0; i < userCount; i++) {
        printf("====================================\n");
        printf("ID: %d\nNama: %s\nTipe: %s\n",
               users[i].id,
               users[i].nama_lengkap,
               users[i].type == ADMIN ? "Admin" : "Member");
    }
}

/**
 * Fungsi untuk menambahkan user baru oleh admin
 * Admin dapat menentukan tipe user (ADMIN atau MEMBER)
 */
void tambahUser() {
    if (userCount >= MAX_USERS) {
        printf("Kapasitas pengguna penuh!\n");
        return;
    }

    User user;
    bool typeValid = false;
    user.id = nextUserId++;

    printf("Masukkan nama lengkap: ");
    fgets(user.nama_lengkap, sizeof(user.nama_lengkap), stdin);
    user.nama_lengkap[strcspn(user.nama_lengkap, "\n")] = 0;

    printf("Masukkan password: ");
    fgets(user.password, sizeof(user.password), stdin);
    user.password[strcspn(user.password, "\n")] = 0;
    encrypt(user.password);

    while (!typeValid) {
        int typeInput;
        printf("Masukkan tipe pengguna\n");
        printf("0: Admin\n1: Member\nPilih salah satu angka di atas: ");
        if (scanf("%d", &typeInput) == 1 && (typeInput == 0 || typeInput == 1)) {
            user.type = typeInput;
            typeValid = true;
        } else {
            printf("Input tidak valid, coba lagi!\n");
            while (getchar() != '\n');
        }
    }

    while (getchar() != '\n');
    users[userCount++] = user;
    printf("Pengguna berhasil ditambahkan!\n");
}

/**
 * Fungsi untuk mengubah data user berdasarkan ID
 * Admin dapat mengubah nama, password, dan tipe user
 */
void updateUser() {
    int id;
    bool typeValid = false;

    if (userCount == 0) {
        printf("Tidak ada data pengguna.\n");
        return;
    }

    tampilUsers();
    printf("\nMasukan ID pengguna yang ingin anda ubah: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < userCount; i++) {
        if (users[i].id == id) {
            printf("Masukkan nama lengkap: ");
            fgets(users[i].nama_lengkap, sizeof(users[i].nama_lengkap), stdin);
            users[i].nama_lengkap[strcspn(users[i].nama_lengkap, "\n")] = 0;

            printf("Masukkan password baru: ");
            fgets(users[i].password, sizeof(users[i].password), stdin);
            users[i].password[strcspn(users[i].password, "\n")] = 0;
            encrypt(users[i].password);

            while (!typeValid) {
                int typeInput;
                printf("Masukkan tipe pengguna\n");
                printf("0: Admin\n1: Member\nPilih salah satu angka di atas: ");
                if (scanf("%d", &typeInput) == 1 && (typeInput == 0 || typeInput == 1)) {
                    users[i].type = typeInput;
                    typeValid = true;
                } else {
                    printf("Input tidak valid, coba lagi!\n");
                    while (getchar() != '\n');
                }
            }

            while (getchar() != '\n');
            printf("\nData pengguna berhasil dirubah\n");
            return;
        }
    }

    printf("\nData pengguna dengan ID %d tidak ditemukan\n", id);
}

/**
 * Fungsi untuk menghapus user berdasarkan ID
 * Array akan digeser ke kiri setelah penghapusan
 */
void hapusUser() {
    int id;
    if (userCount == 0) {
        printf("Tidak ada data pengguna.\n");
        return;
    }

    tampilUsers();
    printf("\nMasukan ID pengguna yang ingin anda hapus: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < userCount; i++) {
        if (users[i].id == id) {
            for (int j = i; j < userCount - 1; j++) {
                users[j] = users[j + 1];
            }
            userCount--;
            printf("Pengguna berhasil dihapus\n");
            return;
        }
    }

    printf("\nData pengguna dengan ID %d tidak ditemukan\n", id);
}

/**
 * Fungsi untuk menampilkan menu member
 * Member hanya dapat melihat daftar buku
 */
void memberMenu() {
    int choice;
    while (true) {
        printf("\n========== MENU MEMBER ==========\n");
        printf("1. Tampilkan Buku\n");
        printf("0. Keluar\n");
        printf("================================\n");
        printf("Pilihan Anda: ");

        if (scanf("%d", &choice) != 1) {
            printf("Input tidak valid. Harus angka.\n");
            while (getchar() != '\n');
            continue;
        }

        getchar();

        switch (choice) {
            case 1:
                tampilBuku();
                break;
            case 0:
                printf("Terima kasih!\n");
                return;
            default:
                printf("Pilihan tidak tersedia. Silakan coba lagi.\n");
        }
    }
}

/**
 * Fungsi untuk menampilkan menu admin
 * Admin dapat mengelola buku dan pengguna
 */
void adminMenu() {
    int choice;
    while (true) {
        printf("\n========== MENU ADMIN ==========\n");
        printf("1. Tambah Buku\n");
        printf("2. Tampilkan Buku\n");
        printf("3. Ubah Data Buku\n");
        printf("4. Hapus Buku\n");
        printf("5. Tambah Pengguna\n");
        printf("6. Tampilkan Pengguna\n");
        printf("7. Ubah Data Pengguna\n");
        printf("8. Hapus Pengguna\n");
        printf("0. Keluar\n");
        printf("================================\n");
        printf("Pilihan Anda: ");

        if (scanf("%d", &choice) != 1) {
            printf("Input tidak valid. Harus angka.\n");
            while (getchar() != '\n');
            continue;
        }

        getchar();

        switch (choice) {
            case 1:
                tambahBuku();
                break;
            case 2:
                tampilBuku();
                break;
            case 3:
                updateBuku();
                break;
            case 4:
                hapusBuku();
                break;
            case 5:
                tambahUser();
                break;
            case 6:
                tampilUsers();
                break;
            case 7:
                updateUser();
                break;
            case 8:
                hapusUser();
                break;
            case 0:
                printf("Terima kasih!\n");
                return;
            default:
                printf("Pilihan tidak tersedia. Silakan coba lagi.\n");
        }
    }
}

/**
 * Fungsi utama sebagai titik masuk program
 * Menyediakan menu login/register dan mengatur alur program
 */
int main() {
    int choice;
    // Inisialisasi admin default
    User admin = {1, "Admin", "password", ADMIN};
    encrypt(admin.password);
    users[userCount++] = admin;
    nextUserId++;

    while (true) {
        printf("\n========== SISTEM PERPUSTAKAAN ==========\n");
        printf("1. Login Member\n");
        printf("2. Register Member\n");
        printf("3. Login Admin\n");
        printf("0. Keluar\n");
        printf("=========================================\n");
        printf("Pilihan Anda: ");

        if (scanf("%d", &choice) != 1) {
            printf("Input tidak valid. Harus angka.\n");
            while (getchar() != '\n');
            continue;
        }

        getchar();

        switch (choice) {
            case 1: {
                int userIndex = loginMember();
                if (userIndex != -1) {
                    printf("Login berhasil! Selamat datang, %s\n", users[userIndex].nama_lengkap);
                    memberMenu();
                } else {
                    printf("Login gagal. Nama atau password salah.\n");
                }
                break;
            }
            case 2:
                registerMember();
                break;
            case 3: {
                int userIndex = loginAdmin();
                if (userIndex != -1) {
                    printf("Login berhasil! Selamat datang, %s\n", users[userIndex].nama_lengkap);
                    adminMenu();
                } else {
                    printf("Login gagal. Nama atau password salah.\n");
                }
                break;
            }
            case 0:
                printf("Terima kasih telah menggunakan program.\n");
                return 0;
            default:
                printf("Pilihan tidak tersedia. Silakan coba lagi.\n");
        }
    }

    return 0;
}
