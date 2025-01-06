/******************
Name:Maya Mattar
ID:213510407
Assignment:ex5
*******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to dynamically read strings
char* getDynamicString() {
    char *str = NULL;
    int len = 0;
    char ch;
    // Clears the input buffer before starting
    while ((ch = getchar()) != '\n' && ch != EOF);

    while ((ch = getchar()) != '\n' && ch != EOF) {
      char *temp = realloc(str, len + 2); // Allocate space for the new character and null terminator
        if (!temp) {
            free(str); // Memory allocation failed
            exit(1);
        }
        str = temp;
        str[len++] = ch;
        str[len] = '\0'; // Null-terminate the string
    }
    return str;
}
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

// Global variables
Playlist **playlists = NULL;
int playlistCount = 0;

// Function prototypes
char* getDynamicString();
void addSong(Playlist *playlist);
void deleteSong(Song *song);
void playSong(Song *song);
void freeSong(Song *song);
void freePlaylist(Playlist *playlist);
void printPlaylistsMenu();
void sort(Song **songs, int songsNum, int criteria);
void managePlaylist(Playlist *playlist);
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
        printf("Now playing %s:\n$ %s $\n", song->title, song->lyrics);
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
    printf("\t4. exit\n");
}
// Sorting function
void sort(Song **songs, int songsNum, int criteria) {
    for (int i = 0; i < songsNum - 1; i++) {
        for (int j = 0; j < songsNum - i - 1; j++) {
            int swap = 0;
            if (criteria == 1 && songs[j]->year > songs[j + 1]->year) {
                swap = 1; // Sort by year
            } else if (criteria == 2 && songs[j]->streams > songs[j + 1]->streams) {
                swap = 1; // Sort by streams (ascending)
            } else if (criteria == 3 && songs[j]->streams < songs[j + 1]->streams) {
                swap = 1; // Sort by streams (descending)
            } else if (criteria == 4 && strcmp(songs[j]->title, songs[j + 1]->title) > 0) {
                swap = 1; // Sort alphabetically
            }
            // Swap songs if needed
            if (swap) {
                Song *temp = songs[j];
                songs[j] = songs[j + 1];
                songs[j + 1] = temp;
            }
        }
    }
}
// Function for add song option
void addSong(Playlist *playlist) {
    printf("Enter song's details\n");
    // Dynamically allocate memory for a new song
    Song **temp = realloc(playlist->songs, (playlist->songsNum + 1) * sizeof(Song *));
    if (!temp) {
        printf("Memory allocation failed!\n");
        free(playlist->songs);
        exit(1);
    }
    playlist->songs = temp;
    playlist->songs[playlist->songsNum] = malloc(sizeof(Song));
    if (!playlist->songs[playlist->songsNum]) {
        printf("Memory allocation failed!\n");
        free(playlist->songs);
        exit(1);
    }
    Song *song = playlist->songs[playlist->songsNum];
    playlist->songsNum++;
    // Input song details dynamically
    printf("Title:\n");
    song->title = getDynamicString();
    printf("Artist:\n");
    song->artist = getDynamicString();
    printf("Year of release:\n");
    scanf("%d", &song->year);
    printf("Lyrics:\n");
    song->lyrics = getDynamicString();
    // Initialize streams to 0
    song->streams = 0;
}
// Manage a specific playlist
void managePlaylist(Playlist *playlist) {
    int option = 0;
    int firstTime = 1; // Flag to track first-time display
    while (1) {
        if (firstTime) {
            printf("playlist %s:\n", playlist->name);
            firstTime = 0; // Reset flag after the first display
        }
        // Print menu options
        printf("\t1. Show Playlist\n");
        printf("\t2. Add Song\n");
        printf("\t3. Delete Song\n");
        printf("\t4. Sort\n");
        printf("\t5. Play\n");
        printf("\t6. exit\n");
        scanf("%d", &option);
        getchar();

        if (option == 1) {  // Show Playlist
            if (playlist->songsNum == 0) {
                printf("The playlist is empty.\n");
            } else {
                int songIndex = -1;
                for (int i = 0; i < playlist->songsNum; i++) {
                    printf("%d. Title: %s\nArtist: %s\nReleased: %d\nStreams: %d\n",
                           i + 1,
                           playlist->songs[i]->title,
                           playlist->songs[i]->artist,
                           playlist->songs[i]->year,
                           playlist->songs[i]->streams);
                    printf("\n");
                }
                // Allow the user to select songs to play until they choose 0
                while (1) {
                    printf("choose a song to play, or 0 to quit:\n");
                    scanf("%d", &songIndex);

                    if (songIndex == 0) {
                        break;  // Exit back to the managePlaylist menu
                    } else if (songIndex > 0 && songIndex <= playlist->songsNum) {
                        // Play the chosen song and increment its streams
                        printf("Now playing %s:\n", playlist->songs[songIndex - 1]->title);
                        printf("$ %s $\n", playlist->songs[songIndex - 1]->lyrics);
                        playlist->songs[songIndex - 1]->streams++;  // Increment the stream count
                    }
                }
            }
        } else if (option == 2) {
            // Call the addSong function to handle adding a song
            addSong(playlist);
        } else if (option == 3) {
            if (playlist->songsNum == 0) {
                printf("The playlist is empty.\n");
            } else {
                // Display all songs for user selection
                for (int i = 0; i < playlist->songsNum; i++) {
                    printf("%d. Title: %s\nArtist: %s\nReleased: %d\nStreams: %d\n",
                        i + 1,
                        playlist->songs[i]->title,
                        playlist->songs[i]->artist,
                        playlist->songs[i]->year,
                        playlist->songs[i]->streams);
