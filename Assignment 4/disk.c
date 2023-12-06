#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int count_numbers(char *input) { // this function counts the length of track
  // it is the number of commas plus 1
  int count = 0;
  for (int i = 0; input[i] != '\0'; ++i) {
    if (input[i] == ',')
      ++count;
  }
  return count + 1;
}

int shortestDist(int arr[], int size, int head) {

  int dist = 1000, newDist, index =0;
  for (int i = 0; i < size; i++) {
    newDist = abs(arr[i] - head);
    if ((newDist < dist) && ((arr[i] - head) != 0)) {
      dist = newDist;
      // printf("distance between %d and %d is %d\n",head,arr[i],newDist);
      index = arr[i];
    }
  }
  return index;
}

void selectionSort(int array[], int size) {
  int i, j, minIndex;

  for (i = 0; i < size - 1; i++) {
    minIndex = i;
    for (j = i + 1; j < size; j++)
      if (array[j] < array[minIndex])
        minIndex = j;

    if (minIndex != i) {
      // Swap the found minimum element with the first element
      int temp = array[i];
      array[i] = array[minIndex];
      array[minIndex] = temp;
    }
  }
}

void delay(int seq[],int ori[],int size){

  float averageDelay = 0;
  int longestDelay = 0,numberOfDelay = 0;
  //int index;


  for(int i = 0; i <size; i++){
    for(int k = 0; k<size;k++){
      if(ori[i] == seq[k]){
        if( i - k < 0){
          // printf("original number %d is at %d, seek number %d is at %d, the delay is %d\n",ori[i],i,seq[k],k,k-i);
          averageDelay=averageDelay+(k-i);
          numberOfDelay++;
          if( k - i > longestDelay){
            longestDelay = k-i;
          }
        }
      }
    }
  }
  printf("\n");
  printf("average delay is %f\n",averageDelay/numberOfDelay);
  printf("longest delay is %d\n",longestDelay);
}

void scan(int arr[], int size) {
  int head = arr[0];
  bool direction = false; // false stands for left, true stands for right.
  int distance, curTrack, tracksTraversed = 0;

  int leftCount = 0, rightCount = 0;
  for (int i = 1; i < size; ++i) {
    if (arr[i] < head) {
      leftCount++;
    } else if (arr[i] > head) {
      rightCount++;
    }
  }

  int left[leftCount], right[rightCount],
      sequence[size]; // arrays for left and right tracks, sequence for the
                      // order.
  int k = 0, l = 0;
  for (int i = 0; i < size; ++i) {
    if (arr[i + 1] < head) {
      left[k] = arr[i + 1];
      k++;
    } else if (arr[i + 1] > head) {
      right[l] = arr[i + 1];
      l++;
    }
  }

  int leftSize = sizeof(left) / sizeof(left[0]);
  int rightSize = sizeof(right) / sizeof(right[0]);

  // printf("left unsorted list");
  // for (int index = 0; index < leftSize; index++){
  // printf("%d \n", left[index]);
  // }

  // printf("right unsorted list");
  // for (int index = 0; index < rightSize; index++){
  // printf("%d \n", right[index]);
  // }

  selectionSort(right, rightSize);
  selectionSort(left, leftSize);

  // printf("left sorted list");
  // for (int index = 0; index < leftSize; index++){
  // printf("%d \n", left[index]);
  // }

  // printf("right sorted list");
  // for (int index = 0; index < rightSize; index++){
  // printf("%d \n", right[index]);
  // }

  int seqIndex = 1;
  for (int i = 2; i > 0; i--) {

    if (direction == false) {
      for (int index = leftSize - 1; index >= 0; index--) {
        curTrack = left[index];
        sequence[seqIndex] = curTrack;
        seqIndex++;
        distance = abs(curTrack - head);
        tracksTraversed += distance;
        head = curTrack;
      }
      direction = true;
    } else if (direction == true) {
      for (int index = 0; index < rightSize; index++) {
        curTrack = right[index];
        sequence[seqIndex] = curTrack;
        seqIndex++;
        distance = abs(curTrack - head);
        tracksTraversed += distance;
        head = curTrack;
      }
      direction = false;
    }
  }
  printf("Total number of tracks traversed in Scan Sequence is : %d\n",
         tracksTraversed);
  sequence[0] = arr[0];
  printf("Scan Seek sequence is: %d",sequence[0]);
  for (int i = 1; i < size; i++) {
    printf(",%d ", sequence[i]);
  }
  printf("\n");
  delay(sequence,arr,size);
}

void SSTF(int arr[], int size) {

  int head = arr[0];
  int newArr[size];
  for(int i = 0;i<size;i++){
    newArr[i]=arr[i];
  }
  int sequence[size];
  int tracksTraversed = 0;
  sequence[0] = head;
  for (int i = 1; i < size; i++) {
    sequence[i] = shortestDist(arr, size, sequence[i - 1]);
    tracksTraversed += abs(sequence[i] - sequence[i - 1]);
    for (int k = 0; k < size; k++) {
      if (arr[k] == sequence[i]) {
        arr[0] = -1000;
        arr[k] = -1000;
      }
    }
  }

  printf("Total number of tracks traversed in SSTF Sequence is : %d\n",
         tracksTraversed);

  printf("SSTF Seek sequence is: ");
  printf("%d", sequence[0]);
  // int total_delay = 0;
  // int delay_count = 0;
  for (int i = 1; i < size; i++) {
    printf(", %d", sequence[i]);
  }
  printf("\n");
  delay(sequence,newArr,size);
}

  int main(int argc, char *argv[]) {
    int n = 0;
    int *disk_queue;
    if (argc == 1) { // if no argument is provided, generate random disk tracks
                     // from 50 to 199
      srand((unsigned)time(NULL));
      n = (rand() % 150) +
          50;          // first randomly generate the length of the disk tracks
      int tracks[200]; // track list
      for (int i = 0; i < 200; i++) {
        tracks[i] = i; // fill the track list with [0,199]
      }
      disk_queue = malloc(n * sizeof(int));
      for (int i = 0; i < n; i++) {
        int j = i + rand() % (200 - i);
        int temp = tracks[i];
        tracks[i] = tracks[j];
        tracks[j] = temp;
        // essentially, shuffle the track list and take the first n tracks
        disk_queue[i] = tracks[i];
      }
    }

    else if (argc == 2) { // if an argument is provided at command line
      // (assume comma-separated, unique, and are non-negative integers less
      // than 200)
      n = count_numbers(argv[1]); // first determine the track length
      disk_queue = malloc(n * sizeof(int));

      char *token =
          strtok(argv[1], ","); // split at comma, and convert to int array
      int count = 0;

      while (token != NULL) {
        // Attempt to convert the token to a long integer
        char *endptr;
        int num = strtol(token, &endptr, 10);

        // Check if the conversion was successful
        if (token != endptr) {
          // Conversion successful, add the number to the array
          disk_queue[count++] = (int)num;
        } else {
          // Conversion failed, token is not a valid number
          printf("Invalid token: %s\n", token);
        }

        // Move to the next token
        token = strtok(NULL, ",");
      }
    }


    printf("--------Original Sequence--------\n");
    // printf("%d tracks\n", n);
    for (int i = 0; i < n; i++) {
      printf("%d ", disk_queue[i]);
    }

    printf("\n");

    printf("--------Scan Algorithm--------\n");

    scan(disk_queue, n);
    printf("\n");

    printf("--------SSTF Algorithm--------\n");

    SSTF(disk_queue, n);
    printf("\n");

  }
