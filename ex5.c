#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Song struct
typedef struct Song {
    char *title;
    char *artist;
    int year;
    char *lyrics;
    int streams;
} Song;

// Playlist struct
typedef struct Playlist {
    char *name;
    Song **songs; // Dynamic array of Song pointers
    int songsNum;
} Playlist;

// Function prototypes
void deleteSong(Song *song);
void playSong(Song *song);
void freeSong(Song *song);
void freePlaylist(Playlist *playlist);
void printPlaylistsMenu();
void sortPlaylist(Song **songs, int songsNum, int criteria);
void mainMenu();

// Function implementations
void deleteSong(Song *song) {
    if (song) {
        free(song->title);
        free(song->artist);
        free(song->lyrics);
        free(song);
        printf("Song deleted successfully.\n");
    }
}

void playSong(Song *song) {
    if (song) {
        song->streams++;
        printf("Now playing %s:\n♪ %s ♪\n", song->title, song->lyrics);
    }
}

void freeSong(Song *song) {
    if (song) {
        free(song->title);
        free(song->artist);
        free(song->lyrics);
        free(song);
    }
}

void freePlaylist(Playlist *playlist) {
    if (playlist) {
        for (int i = 0; i < playlist->songsNum; i++) {
            freeSong(playlist->songs[i]);
        }
        free(playlist->songs);
        free(playlist->name);
        free(playlist);
    }
}

void printPlaylistsMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n");
    printf("\t2. Add playlist\n");
    printf("\t3. Remove playlist\n");
    printf("\t4. Exit\n");
}

void sortPlaylist(Song **songs, int songsNum, int criteria) {
    // Sorting logic based on criteria (1: title, 2: year, 3: streams)
    for (int i = 0; i < songsNum - 1; i++) {
        for (int j = i + 1; j < songsNum; j++) {
            int swap = 0;
            if (criteria == 1 && strcmp(songs[i]->title, songs[j]->title) > 0) {
                swap = 1;
            } else if (criteria == 2 && songs[i]->year > songs[j]->year) {
                swap = 1;
            } else if (criteria == 3 && songs[i]->streams < songs[j]->streams) {
                swap = 1;
            }
            if (swap) {
                Song *temp = songs[i];
                songs[i] = songs[j];
                songs[j] = temp;
            }
        }
    }
    printf("Sorted\n");
}

void mainMenu() {
    int choice;
    Playlist **playlists = NULL;
    int playlistCount = 0;

    while (1) {
        printPlaylistsMenu();
        scanf("%d", &choice);

        if (choice == 1) {
            // Watch playlists
            if (playlistCount == 0) {
                printf("No playlists available.\n");
            } else {
                for (int i = 0; i < playlistCount; i++) {
                    printf("%d. %s\n", i + 1, playlists[i]->name);
                }
            }
        } else if (choice == 2) {
            // Add playlist
            playlists = realloc(playlists, (playlistCount + 1) * sizeof(Playlist *));
            playlists[playlistCount] = malloc(sizeof(Playlist));
            playlists[playlistCount]->songsNum = 0;
            playlists[playlistCount]->songs = NULL;

            printf("Enter playlist name: ");
            char buffer[100];
            scanf("%s", buffer);
            playlists[playlistCount]->name = strdup(buffer);
            playlistCount++;
            printf("Playlist added successfully.\n");
        } else if (choice == 3) {
            // Remove playlist
            if (playlistCount == 0) {
                printf("No playlists to remove.\n");
            } else {
                printf("Enter the number of the playlist to remove: ");
                int pIndex;
                scanf("%d", &pIndex);
                if (pIndex > 0 && pIndex <= playlistCount) {
                    freePlaylist(playlists[pIndex - 1]);
                    for (int i = pIndex - 1; i < playlistCount - 1; i++) {
                        playlists[i] = playlists[i + 1];
                    }
                    playlistCount--;
                    playlists = realloc(playlists, playlistCount * sizeof(Playlist *));
                    printf("Playlist removed successfully.\n");
                } else {
                    printf("Invalid choice.\n");
                }
            }
        } else if (choice == 4) {
            // Exit
            for (int i = 0; i < playlistCount; i++) {
                freePlaylist(playlists[i]);
            }
            free(playlists);
            printf("Goodbye!\n");
            break;
        } else {
            printf("Invalid option.\n");
        }
    }
}

int main() {
    mainMenu();
    return 0;
}
