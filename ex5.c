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

// Comparison functions for sorting songs
int cmpByYear(Song *a, Song *b) {
    return a->year - b->year;
}

int cmpByStreamsAsc(Song *a, Song *b) {
    return a->streams - b->streams;
}

int cmpByStreamsDesc(Song *a, Song *b) {
    return b->streams - a->streams;
}

int cmpAlphabetical(Song *a, Song *b) {
    return strcmp(a->title, b->title);
}

// Generic bubble sort function for songs array
void sort(Song **arr, int n, int (*cmp)(Song *, Song *)) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (cmp(arr[j], arr[j + 1]) > 0) {
                Song *temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    printf("sorted\n");
}

// Print the main playlists menu options
void printPlaylistsMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}

// Function to manage user interaction with playlists
void WatchPlaylists(Playlist *p, int count) {
    printf("Choose a playlist:\n");
    for(int i = 0; i < count; i++) {
        printf("\t%d. %s\n", i + 1, p[i].name);
    }
    printf("\t%d. Back to main menu\n", count + 1);

    int choice1;
    scanf("%d", &choice1);

    // If user selects a valid playlist
    if (choice1 != count + 1){
        int choice2;
        do {
            // Show playlist options menu
            printf("playlist %s:\n", p[choice1- 1].name);
            printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n");
            scanf("%d", &choice2);

            switch (choice2) {
                case 1:
                    // Display all songs in the selected playlist
                    for (int i = 0; i < p[choice1 - 1].songsNum; i++) {
                        printf("%d. Title: %s\n", i + 1, p[choice1 - 1].songs[i]->title);
                        printf("    Artist: %s\n", p[choice1 - 1].songs[i]->artist);
                        printf("    Released: %d\n", p[choice1 - 1].songs[i]->year);
                        printf("    Streams: %d\n", p[choice1 - 1].songs[i]->streams);
                    }

                    // Let user choose a song to play, or quit
                    int choice3;
                    do {
                        printf("choose a song to play, or 0 to quit:\n");
                        scanf("%d", &choice3);

                        if (choice3 == 0 || p[choice1 - 1].songsNum == 0) break;

                        // Show lyrics and increment stream count
                        printf("Now playing %s:\n", p[choice1 - 1].songs[choice3 - 1]->title);
                        printf("$ %s $\n", p[choice1 - 1].songs[choice3 - 1]->lyrics);
                        p[choice1 - 1].songs[choice3 - 1]->streams++;
                    } while(choice3 != 0);
                    break;
                case 2:
                    // Add a new song to the playlist
                    printf("Enter song's details:\n");

                    char title[100];
                    char artist[100];
                    int year;
                    char lyrics[1000];
                    int streams;

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

                    // Reallocate memory for songs array
                    p[choice1 - 1].songs = realloc(p[choice1 - 1].songs, sizeof(Song *) * (p[choice1 - 1].songsNum + 1));
                    if (p[choice1 - 1].songs == NULL) exit(1);

                    // Allocate and copy new song data
                    p[choice1 - 1].songs[p[choice1 - 1].songsNum] = malloc(sizeof(Song));
                    if (p[choice1 - 1].songs[p[choice1 - 1].songsNum] == NULL) exit(1);

                    p[choice1 - 1].songs[p[choice1 - 1].songsNum]->title = malloc(strlen(title) + 1);
                    strcpy(p[choice1 - 1].songs[p[choice1 - 1].songsNum]->title, title);

                    p[choice1 - 1].songs[p[choice1 - 1].songsNum]->artist = malloc(strlen(artist) + 1);
                    strcpy(p[choice1 - 1].songs[p[choice1 - 1].songsNum]->artist, artist);

                    p[choice1 - 1].songs[p[choice1 - 1].songsNum]->lyrics = malloc(strlen(lyrics) + 1);
                    strcpy(p[choice1 - 1].songs[p[choice1 - 1].songsNum]->lyrics, lyrics);

                    p[choice1 - 1].songs[p[choice1 - 1].songsNum]->year = year;
                    p[choice1 - 1].songs[p[choice1 - 1].songsNum]->streams = streams;

                    p[choice1 - 1].songsNum++;

                    break;
                case 3:
                    // Delete a song from the playlist
                    for (int i = 0; i < p[choice1 - 1].songsNum; i++) {
                        printf("%d. Title: %s\n", i + 1, p[choice1 - 1].songs[i]->title);
                        printf("    Artist: %s\n", p[choice1 - 1].songs[i]->artist);
                        printf("    Released: %d\n", p[choice1 - 1].songs[i]->year);
                        printf("    Streams: %d\n", p[choice1 - 1].songs[i]->streams);
                    }

                    int choice4;
                    printf("choose a song to delete, or 0 to quit:\n");
                    scanf("%d", &choice4);
                    if (choice4 == 0) break;

                    // Free memory of the selected song
                    free(p[choice1 - 1].songs[choice4 - 1]->title);
                    free(p[choice1 - 1].songs[choice4 - 1]->artist);
                    free(p[choice1 - 1].songs[choice4 - 1]->lyrics);
                    free(p[choice1 - 1].songs[choice4 - 1]);

                    // Shift remaining songs left
                    for (int i = choice4 - 1; i < p[choice1 - 1].songsNum - 1; i++) {
                        p[choice1 - 1].songs[i] = p[choice1 - 1].songs[i + 1];
                    }

                    p[choice1 - 1].songsNum--;

                    // Adjust songs array size or free if empty
                    if (p[choice1 - 1].songsNum == 0) {
                        free(p[choice1 - 1].songs);
                        p[choice1 - 1].songs = NULL;
                    } else {
                        p[choice1 - 1].songs = realloc(p[choice1 - 1].songs,
                            sizeof(Song *) * p[choice1 - 1].songsNum);
                        if (!p[choice1 - 1].songs) exit(1);
                    }

                    printf("song deleted successfully.\n");
                    break;
                case 4:
                    // Sort songs with user's chosen criteria
                    printf("choose:\n");
                    printf("1. sort by year\n");
                    printf("2. sort by streams - ascending order\n");
                    printf("3. sort by streams - descending order\n");
                    printf("4. sort alphabetically\n");

                    int choice5;
                    scanf("%d", &choice5);

                    int (*cmpFunc)(Song *, Song *);
                    switch (choice5) {
                        case 1:
                            cmpFunc = cmpByYear;
                            break;
                        case 2:
                            cmpFunc = cmpByStreamsAsc;
                            break;
                        case 3:
                            cmpFunc = cmpByStreamsDesc;
                            break;
                        case 4:
                        default:
                            cmpFunc = cmpAlphabetical;
                    }

                    sort(p[choice1 - 1].songs, p[choice1 - 1].songsNum, cmpFunc);
                    break;
                case 5:
                    // Play all songs in the playlist, incrementing streams
                    for (int i = 0; i < p[choice1 - 1].songsNum; i++) {
                        printf("Now playing %s:\n", p[choice1 - 1].songs[i]->title);
                        printf("$ %s $\n", p[choice1 - 1].songs[i]->lyrics);
                        printf("\n");
                        p[choice1 - 1].songs[i]->streams++;
                    }
                    break;
                case 6:
                    // Exit playlist menu
                    break;
                default:
                    printf("Invalid option\n");
            }
        }while(choice2 != 6);
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
                WatchPlaylists(p, count);
                break;
            case 2:
                // Add a new playlist
                printf("Enter playlist's name:\n");

                p = realloc(p, sizeof(Playlist) * (count + 1));
                if (p == NULL) exit(1);

                p[count].name = malloc(sizeof(char) * 100);
                if (p[count].name == NULL) exit(1);

                scanf("%99s", p[count].name);

                p[count].songs = NULL;
                p[count].songsNum = 0;

                count++;
                break;
            case 3:
                // Remove a playlist
                printf("Choose a playlist:\n");
                for(int i = 0; i < count; i++) {
                    printf("\t%d. %s\n", i + 1, p[i].name);
                }
                printf("\t%d. Back to main menu\n", count + 1);

                int choice2;
                scanf("%d", &choice2);

                if (choice2 == count + 1) break;

                // Free all songs inside the playlist
                for (int i = 0; i < p[choice2 - 1].songsNum; i++) {
                    free(p[choice2 - 1].songs[i]->title);
                    free(p[choice2 - 1].songs[i]->artist);
                    free(p[choice2 - 1].songs[i]->lyrics);
                    free(p[choice2 - 1].songs[i]);
                }
                free(p[choice2 - 1].songs);
                free(p[choice2 - 1].name);

                // Shift playlists left and decrease count
                for (int i = choice2 - 1; i < count - 1; i++) {
                    p[i] = p[i + 1];
                }

                count--;

                if (count == 0) {
                    free(p);
                    p = NULL;
                } else {
                    // Resize playlists array
                    p = realloc(p, sizeof(Playlist) * count);
                    if (p == NULL) exit(1);
                }

                printf("playlist deleted.\n");
                break;
            case 4:
                // Exit program
                break;
            default:
                printf("Invalid option\n");
        }
    }while(choice != 4);
    printf("Goodbye!\n");

    // Free all remaining allocated memory before exit
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
