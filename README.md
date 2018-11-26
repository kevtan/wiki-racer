# Wikipedia Racer
This program was inspired from the "popular" game called WikiRacer where people try to get from one random Wikipedia page to another random Wikipedia page in the shorted amount of time.

## Implementation
This pseudocode for this algorithm works in the following way:
1. Find the complete link set (the set of all links) of the initial and ending page.
2. Enqueue into a priority queue the first page.
3. While the priority queue is not empty
   1. Dequeue the highest priority element (the potential ladder whose last page has the most links in common with the target page).
   2. Find the link set for the last page in the potential ladder.
   3. If one of the links is equal to the target page, return.
   4. Else, enqueue new potential ladders to priority queue with the base as the initial ladder and new element being one of the links in the linkset.
4. Print out a ladder from page one to page two, otherwise print out nothing.

## Credits
This project was part of CS 106L (Standard C++ Programming) taught by Ali Malik in 2018 at Stanford University.
