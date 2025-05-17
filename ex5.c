/******************
Name: Liam Homay
ID: 333087807
Assignment: ex5
*******************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Song {
    char *title;
    char *artist;
    int year;
    char *lyrics;
    int streams;
} Song;

typedef struct Playlist {
    char *name;
    Song **songs;
    int songsNum;
} Playlist;

// השוואת שירים לסידור
int cmpByYear(Song *a, Song *b) { return a->year - b->year; }
int cmpByStreamsAsc(Song *a, Song *b) { return a->streams - b->streams; }
int cmpByStreamsDesc(Song *a, Song *b) { return b->streams - a->streams; }
int cmpAlphabetical(Song *a, Song *b) { return strcmp(a->title, b->title); }

// מיון בועות
void sort(Song **arr, int n, int (*cmp)(Song *, Song *)) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (cmp(arr[j], arr[j + 1]) > 0) {
                Song *temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
}

// הצגת תפריט ראשי
void printPlaylistsMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}

// פונקציה להצגת כל השירים בפלייליסט
void printSongs(Playlist *pl) {
    for (int i = 0; i < pl->songsNum; i++) {
        printf("%d. Title: %s\n", i + 1, pl->songs[i]->title);
        printf("    Artist: %s\n", pl->songs[i]->artist);
        printf("    Released: %d\n", pl->songs[i]->year);
        printf("    Streams: %d\n", pl->songs[i]->streams);
    }
}

// פונקציה שמקבלת קלט שיר מהמשתמש ומחזירה פוינטר לשיר חדש
Song *createSongFromInput() {
    char title[100], artist[100], lyrics[1000];
    int year, streams;

    printf("Enter song's details:\n");

    printf("Title:\n");
    scanf(" %[^\n]", title);

    printf("Artist:\n");
    scanf(" %[^\n]", artist);

    printf("Year of release:\n");
    scanf("%d", &year);

    printf("Lyrics:\n");
    scanf(" %[^\n]", lyrics);

    printf("Streams:\n");
    scanf("%d", &streams);

    Song *newSong = malloc(sizeof(Song));
    if (!newSong) exit(1);

    newSong->title = malloc(strlen(title) + 1);
    newSong->artist = malloc(strlen(artist) + 1);
    newSong->lyrics = malloc(strlen(lyrics) + 1);

    if (!newSong->title || !newSong->artist || !newSong->lyrics) exit(1);

    strcpy(newSong->title, title);
    strcpy(newSong->artist, artist);
    strcpy(newSong->lyrics, lyrics);

    newSong->year = year;
    newSong->streams = streams;

    return newSong;
}

// הוספת שיר לפלייליסט
void addSongToPlaylist(Playlist *pl) {
    Song *newSong = createSongFromInput();

    pl->songs = realloc(pl->songs, sizeof(Song *) * (pl->songsNum + 1));
    if (!pl->songs) exit(1);

    pl->songs[pl->songsNum] = newSong;
    pl->songsNum++;
}

// מחיקת שיר מהפלייליסט
void deleteSongFromPlaylist(Playlist *pl) {
    if (pl->songsNum == 0) {
        printf("Playlist is empty.\n");
        return;
    }

    printSongs(pl);

    int choice;
    printf("Choose a song to delete, or 0 to quit:\n");
    scanf("%d", &choice);

    if (choice == 0) return;
    if (choice < 1 || choice > pl->songsNum) {
        printf("Invalid choice.\n");
        return;
    }

    int idx = choice - 1;

    free(pl->songs[idx]->title);
    free(pl->songs[idx]->artist);
    free(pl->songs[idx]->lyrics);
    free(pl->songs[idx]);

    for (int i = idx; i < pl->songsNum - 1; i++) {
        pl->songs[i] = pl->songs[i + 1];
    }

    pl->songsNum--;

    if (pl->songsNum == 0) {
        free(pl->songs);
        pl->songs = NULL;
    } else {
        pl->songs = realloc(pl->songs, sizeof(Song *) * pl->songsNum);
        if (!pl->songs) exit(1);
    }

    printf("Song deleted successfully.\n");
}

// השמעת שירים - מאפשר לבחור שיר או להאזין לכולם ברצף
void playSongs(Playlist *pl) {
    if (pl->songsNum == 0) {
        printf("Playlist is empty.\n");
        return;
    }

    int choice;
    do {
        printSongs(pl);
        printf("Choose a song to play, or 0 to quit:\n");
        scanf("%d", &choice);

        if (choice == 0) break;
        if (choice < 1 || choice > pl->songsNum) {
            printf("Invalid choice.\n");
            continue;
        }

        Song *song = pl->songs[choice - 1];
        printf("Now playing %s:\n", song->title);
        printf("$ %s $\n", song->lyrics);
        song->streams++;

    } while (choice != 0);
}

// פונקציה לטיפול בפלייליסט נבחר - תפריט שנפתח אחרי בחירת פלייליסט
void watchPlaylistMenu(Playlist *p, int count) {
    printf("Choose a playlist:\n");
    for (int i = 0; i < count; i++) {
        printf("\t%d. %s\n", i + 1, p[i].name);
    }
    printf("\t%d. Back to main menu\n", count + 1);

    int choice1;
    scanf("%d", &choice1);

    if (choice1 == count + 1) return;
    if (choice1 < 1 || choice1 > count) {
        printf("Invalid playlist choice.\n");
        return;
    }

    Playlist *pl = &p[choice1 - 1];

    int choice2;
    do {
        printf("Playlist %s:\n", pl->name);
        printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. Exit\n");
        scanf("%d", &choice2);

        switch (choice2) {
            case 1:
                printSongs(pl);
                break;
            case 2:
                addSongToPlaylist(pl);
                break;
            case 3:
                deleteSongFromPlaylist(pl);
                break;
            case 4: {
                printf("Choose sorting method:\n");
                printf("1. Sort by year\n");
                printf("2. Sort by streams - ascending\n");
                printf("3. Sort by streams - descending\n");
                printf("4. Sort alphabetically\n");

                int choiceSort;
                scanf("%d", &choiceSort);

                int (*cmpFunc)(Song *, Song *);
                switch (choiceSort) {
                    case 1: cmpFunc = cmpByYear; break;
                    case 2: cmpFunc = cmpByStreamsAsc; break;
                    case 3: cmpFunc = cmpByStreamsDesc; break;
                    case 4:
                    default:
                        cmpFunc = cmpAlphabetical;
                }
                sort(pl->songs, pl->songsNum, cmpFunc);
                break;
            }
            case 5:
                playSongs(pl);
                break;
            case 6:
                break;
            default:
                printf("Invalid option\n");
        }
    } while (choice2 != 6);
}

// פונקציה למחיקת פלייליסט (כולל שחרור זיכרון)
void deletePlaylist(Playlist *p, int idx, int *count) {
    for (int i = 0; i < p[idx].songsNum; i++) {
        free(p[idx].songs[i]->title);
        free(p[idx].songs[i]->artist);
        free(p[idx].songs[i]->lyrics);
        free(p[idx].songs[i]);
    }
    free(p[idx].songs);
    free(p[idx].name);

    for (int i = idx; i < (*count) - 1; i++) {
        p[i] = p[i + 1];
    }
    (*count)--;

    if (*count == 0) {
        free(p);
        p = NULL;
    } else {
        p = realloc(p, sizeof(Playlist) * (*count));
        if (!p) exit(1);
    }
}

int main() {
    int count = 0;
    Playlist *p = NULL;

    int choice;
    do {
        printPlaylistsMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (count == 0) {
                    printf("No playlists available.\n");
                } else {
                    watchPlaylistMenu(p, count);
                }
                break;

            case 2: {
                printf("Enter playlist's name:\n");

                p = realloc(p, sizeof(Playlist) * (count + 1));
                if (p == NULL) exit(1);

                char name[100];
                scanf("%99s", name);

                p[count].name = malloc(strlen(name) + 1);
                if (p[count].name == NULL) exit(1);

                strcpy(p[count].name, name);

                p[count].songs = NULL;
                p[count].songsNum = 0;

                count++;
                break;
            }
            case 3:
                if (count == 0) {
                    printf("No playlists available to delete.\n");
                    break;
                }

                printf("Choose a playlist:\n");
                for (int i = 0; i < count; i++) {
                    printf("\t%d. %s\n", i + 1, p[i].name);
                }
                printf("\t%d. Back to main menu\n", count + 1);

                int choice2;
                scanf("%d", &choice2);

                if (choice2 == count + 1) break;
                if (choice2 < 1 || choice2 > count) {
                    printf("Invalid playlist choice.\n");
                    break;
                }

                deletePlaylist(p, choice2 - 1, &count);
                printf("Playlist deleted.\n");
                break;

            case 4:
                printf("Goodbye!\n");
                break;

            default:
                printf("Invalid option\n");
        }
    } while (choice != 4);

    // ניקוי זיכרון בסיום התכנית
    if (p != NULL) {
        for (int i = 0; i < count; i++) {
            for (int j = 0; j < p[i].songsNum; j++) {
                free(p[i].songs[j]->title);
                free(p[i].songs[j]->artist);
                free(p[i].songs[j]->lyrics);
                free(p[i].songs[j]);
            }
            free(p[i].songs);
            free(p[i].name);
        }
        free(p);
    }
    return 0;
}
