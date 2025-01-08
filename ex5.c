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
    while (getchar() != '\n');
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
            // Pointer to the playlist name
            int length = strlen(playlist->name);

            // Create a temporary buffer to clean the playlist name
            char tmp[length + 1];
            strcpy(tmp, playlist->name);

            // Remove trailing spaces (but not the colon)
            if (tmp[length - 1] == ' ') {
                tmp[length - 1] = '\0';
            }

            // Print the cleaned-up playlist name
            printf("playlist %s:\n",tmp);
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
        while (getchar() != '\n');

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
                    getchar();
                    if (songIndex == 0) {
                        break;
                    } else if (songIndex > 0 && songIndex <= playlist->songsNum) {
                        // Pointer to the selected song
                        Song *song = playlist->songs[songIndex - 1];
                        int length = strlen(song->title);
                        // Create a temporary buffer to clean up the title
                        char tmp[length + 1];
                        strcpy(tmp, song->title);
                        // Remove trailing spaces (but not the colon)
                        if (tmp[length - 1] == ' ') {
                            tmp[length - 1] = '\0';
                        }
                        // Print the cleaned-up title with the song index
                        printf("Now playing %s:\n", tmp);
                        printf("$ %s $\n", song->lyrics);
                        // Increment the stream count
                        song->streams++;
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
                    printf("\n");
                }
                printf("choose a song to delete, or 0 to quit:\n");
                int songIndex;
                scanf("%d", &songIndex);
                while (getchar() != '\n');
                // Check if user wants to quit
                if (songIndex == 0) {
                    return;
                }
                // Free memory for the selected song
                freeSong(playlist->songs[songIndex - 1]);
                // Shift remaining songs in the array
                for (int i = songIndex - 1; i < playlist->songsNum - 1; i++) {
                    playlist->songs[i] = playlist->songs[i + 1];
                }
                playlist->songsNum--; // Decrement song count
                playlist->songs = realloc(playlist->songs, playlist->songsNum * sizeof(Song *));
                if (playlist->songsNum > 0 && !playlist->songs) {
                    printf("Memory allocation error.\n");
                    exit(1);
                }
                printf("Song deleted successfully.\n");
            }
        } else if (option == 4) {
            printf("choose:\n");
            printf("1. sort by year\n");
            printf("2. sort by streams - ascending order\n");
            printf("3. sort by streams - descending order\n");
            printf("4. sort alphabetically\n");
            int sortOption;
            scanf("%d", &sortOption);
            while (getchar() != '\n');
            if (sortOption >= 1 && sortOption <= 4) {
                sort(playlist->songs, playlist->songsNum, sortOption);
                printf("sorted\n");
            } else {
                sort(playlist->songs, playlist->songsNum, 4);
                printf("sorted\n");
            }
            continue;
        } else if (option == 5) {
            if (playlist->songsNum == 0) {
                printf("The playlist is empty.\n");
            } else {
                for (int i = 0; i < playlist->songsNum; i++) {
                    // Pointer to the song in index i
                    Song *song = playlist->songs[i];
                    int length = strlen(song->title);

                    // Create a temporary buffer to clean the title
                    char tmp[length + 1];
                    strcpy(tmp, song->title);

                    // Remove trailing spaces (but not the colon)
                    if (tmp[length - 1] == ' ') {
                        tmp[length - 1] = '\0';
                    }
                    // Print the cleaned-up title
                    printf("Now playing %s:\n", tmp);
                    printf("$ %s $\n", song->lyrics);

                    // Increment the stream count
                    song->streams++;
                }
            }
        } else if (option == 6) {
            return; // Back to choose a playlist menu
        }
        else {
            break;
        }
    }
}
// Main menu
void mainMenu() {
    int choice = 0;
    int subChoice;
    while (1) {
        printf("Please Choose:\n");
        printf("\t1. Watch playlists\n");
        printf("\t2. Add playlist\n");
        printf("\t3. Remove playlist\n");
        printf("\t4. exit\n");

        scanf("%d", &choice);
        while (getchar() != '\n');

        if (choice == 1) {
            if (playlistCount == 0) {
                printf("Choose a playlist:\n");
                printf("\t1. Back to main menu\n");
                scanf("%d", &subChoice);
                while (getchar() != '\n');
            } else {
                while (1) {
                    printf("Choose a playlist:\n");
                    for (int i = 0; i < playlistCount; i++) {
                        printf("\t%d. %s\n", i + 1, playlists[i]->name);
                    }
                    printf("\t%d. Back to main menu\n", playlistCount + 1);
                    scanf("\t%d", &subChoice);
                    while (getchar() != '\n');
                    if (subChoice == playlistCount + 1) {
                        break;
                    } else if (subChoice > 0 && subChoice <= playlistCount) {
                        managePlaylist(playlists[subChoice - 1]);
                    }
                }
            }
        } else if (choice == 2) {
            playlists = realloc(playlists, (playlistCount + 1) * sizeof(Playlist *));
            if (!playlists) {
                printf("Memory allocation error.\n");
                exit(1);
            }
            playlists[playlistCount] = malloc(sizeof(Playlist));
            if (!playlists[playlistCount]) {
                printf("Memory allocation error.\n");
                exit(1);
            }
            playlists[playlistCount]->songs = NULL;
            playlists[playlistCount]->songsNum = 0;

            printf("Enter playlist's name:\n");
            playlists[playlistCount]->name = getDynamicString();
            playlistCount++;
        } else if (choice == 3) {
            while (1) {
                printf("Choose a playlist:\n");
                // Display playlists with the "Back to main menu" option
                for (int i = 0; i < playlistCount; i++) {
                    printf("\t%d. %s\n", i + 1, playlists[i]->name);
                }
                printf("\t%d. Back to main menu\n", playlistCount + 1);
                // Prompt user to select a playlist to delete
                int playlistIndex;
                scanf("%d", &playlistIndex);
                while (getchar() != '\n');
                // Handle "Back to main menu" selection
                if (playlistIndex == playlistCount + 1) {
                    break; // Return to the main menu
                } else if (playlistIndex > 0 && playlistIndex <= playlistCount) {
                    // Delete the selected playlist
                    for (int i = 0; i < playlists[playlistIndex - 1]->songsNum; i++) {
                        freeSong(playlists[playlistIndex - 1]->songs[i]); // Free all songs
                    }
                    free(playlists[playlistIndex - 1]->songs);  // Free the playlist's song array
                    free(playlists[playlistIndex - 1]->name);   // Free the playlist name
                    free(playlists[playlistIndex - 1]);         // Free the playlist structure
                    // Shift remaining playlists in the array
                    for (int i = playlistIndex - 1; i < playlistCount - 1; i++) {
                        playlists[i] = playlists[i + 1];
                    }
                    playlistCount--; // Decrement playlist count
                    playlists = realloc(playlists, playlistCount * sizeof(Playlist *));
                    if (playlistCount > 0 && !playlists) {
                        printf("Memory allocation failed during resize!\n");
                        exit(1);
                    }
                    printf("Playlist deleted.\n");
                    break;
                }
            }
        } else if (choice == 4) {
            for (int i = 0; i < playlistCount; i++) {
                free(playlists[i]->name);
                for (int j = 0; j < playlists[i]->songsNum; j++) {
                    free(playlists[i]->songs[j]->title);
                    free(playlists[i]->songs[j]->artist);
                    free(playlists[i]->songs[j]->lyrics);
                    free(playlists[i]->songs[j]);
                }
                free(playlists[i]->songs);
                free(playlists[i]);
            }
            free(playlists);
            printf("Goodbye!\n");
            exit(0);
        } else {
            break;
        }
    }
}
// Main function
int main() {
    mainMenu();
    return 0;
}
