/*********************************************************************
*   Example framework for assignment 9 IPC031.
*   Content:
*   - the type definitions for the music database for *arrays* (convert this yourself to vector, see part 1)
*   - ordering relations on Track;
*   - reading Tracks.txt file;
*   - functions and data structures from lecture #9 IPC031:
*   - sorting algorithms insertion_sort, selection_sort, bubble_sort
*     (based on *arrays*, convert this yourself to vector)
*   - main that reads Tracks.txt and calls a sorting algorithm.
**********************************************************************/
//Assignment 9 Bonus
//Paolo Scattolin s1023775
//Johan Urban s1024726
#define NDEBUG
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cassert>
#include <algorithm>
#include <string.h>

using namespace std;


/*********************************************************************
*   type definitions and global array songs:
**********************************************************************/
struct Length
{
	int minutes;							// #minutes (0..)
	int seconds;							// #seconds (0..59)
};
struct Track
{
	string artist;                          // artist name
	string cd;                              // cd title
	int    year;                            // year of appearance
	int    track_no;						// track number
	string title;                           // track title
	string tags;                            // track tags (separated by ,)
	Length time;							// track length
	string country;                         // countr(y/ies) of artist (separated by ,)
};
int iteration = 0;

vector<Track> songs ;

/************************************************************************
*   Ordering relations op Track:
*      define < and == for Length and Track yourself, the other
*      ordering relations (>, <=, >=, >) follow from these automatically.
************************************************************************/
bool operator<(const Length& a, const Length& b)
{
    iteration++;
    if (a.minutes == b.minutes)
        return a.seconds < b.seconds;
    else return a.minutes < b.minutes;

}
bool operator==(const Length& a, const Length& b)
{
    iteration++;
    return (a.minutes == b.minutes && a.seconds == b.seconds) ;   /* implement a correct == ordering on Track values */
}
bool operator>(const Length& a, const Length& b)
{
	return b < a ;
}
bool operator<=(const Length& a, const Length& b)
{
	return !(b < a) ;
}
bool operator>=(const Length& a, const Length& b)
{
	return b <= a ;
}
bool operator<(const Track& a, const Track& b)
{
    iteration++;
    if(a.artist == b.artist)
    {
         if(a.cd == b.cd)
         {
             if(a.year == b.year)
             {
                return a.track_no < b.track_no;
             }else return a.year < b.year;

         }else return a.cd < b.cd;

   }else return a.artist < b.artist ;
    //return a.time < b.time ;
}
bool operator==(const Track& a, const Track& b)
{
    iteration++;
    return (a.artist == b.artist && a.cd == b.cd && a.year == b.year && a.track_no == b.track_no) ;
    //return a.time == b.time;
}
bool operator>(const Track& a, const Track& b)
{
	return b < a ;
}
bool operator<=(const Track& a, const Track& b)
{
	return !(b < a) ;
}
bool operator>=(const Track& a, const Track& b)
{
	return b <= a ;
}

/************************************************************************
*   Reading Tracks.txt file:
************************************************************************/
istream& operator>> (istream& in, Length& length)
{// Pre-condition:
    assert (true) ;
/*  Post-condition:
    a value for length has been obtained from in: first minutes, followed by ':', followed by seconds.
*/
    char colon ;
    in >> length.minutes >> colon >> length.seconds ;
    return in ;
}
ostream& operator<< (ostream& out, const Length length)
{// Pre-condition:
    assert (true) ;
/*  Post-condition:
    length is sent to out as: minutes, ':', seconds (at least 2 digits)
*/
    out << length.minutes << ':';
	if (length.seconds < 10)
		out << '0';
	out << length.seconds;
	return out ;
}
void skip_to_next_newline (istream& infile)
{// pre-condition:
    assert (true) ;
/*  post-condition:
    infile has been read up to and including the next newline character.
*/
    string remains ;
    getline (infile, remains) ;
}
istream& operator>> (istream& infile, Track& track)
{// pre-condition:
    assert (true) ;
/*  post-condition:
    infile has been read up to and including all members of one Track entry, including the last separating newline.
    These members are stored in track.
*/
    getline(infile,track.artist);       // artist
    getline(infile,track.cd);           // cd
    infile >> track.year ;              // year
    skip_to_next_newline(infile) ;
    infile >> track.track_no ;          // track number
    skip_to_next_newline(infile) ;
    getline(infile,track.title);        // track title
    getline(infile,track.tags);         // track tags
    infile >> track.time ;              // track time
    skip_to_next_newline(infile) ;
    getline(infile,track.country);      // country
    skip_to_next_newline(infile) ;
    return infile ;
}
// not necessary for assignment (because of show_track function), but included anyway because of symmetry:
ostream& operator<< (ostream& out, const Track track)
{// pre-condition:
    assert (true) ;
/*  post-condition:
    out has emitted all members of track, including the last separating newline.
*/
    out << track.artist   << endl ;
    out << track.cd       << endl ;
    out << track.year     << endl ;
    out << track.track_no << endl ;
    out << track.title    << endl ;
    out << track.tags     << endl ;
    out << track.time     << endl ;
    out << track.country  << endl ;
    out << endl ;
    return out ;
}
// NOTE: this version uses *array* implementation, convert to vector yourself
int read_songs (ifstream& infile, vector <Track>& songs)
{// pre-condition:
    assert (infile.is_open ()) ;
/*  post-condition:
    All tracks in infile have been read and stored in the same order in songs.
    The result is the number of tracks that have been read.
*/
    int i = 0 ;
    do {
        Track song ;
        infile >> song ;
        if (infile)
        {
            songs.push_back(song);
            i++;
        }
    }
    while (infile);

    return i ;
}
int read_file (string filename)
{// pre-condition:
    assert (songs.size() == 0) ;
/*  post-condition:
    If the result is -1, then no file could be opened, and songs is unchanged.
    Otherwise, the result is zero or positive, and songs contains all the found tracks in the
    file that corresponds with filename.
*/
    ifstream songsDBS (filename.c_str());
    if (!songsDBS)
    {
        cout << "Could not open '" << filename << "'." << endl;
        return -1;
    }
    cout << "Reading '" << filename << "'." << endl;
	const int NO_OF_SONGS = read_songs (songsDBS, songs);
	songsDBS.close();
	cout << "Read " << NO_OF_SONGS << " tracks." << endl;
	return NO_OF_SONGS;
}
// NOTE: this version uses *array* implementation, convert to vector yourself
void show_all_tracks (vector <Track> songs, int no_of_songs)
{// pre-condition:

/*  post-condition:
    songs[0] ... songs[no_of_songs-1] have been displayed to cout.
*/
    for (int i = 0 ; i < no_of_songs ; i++)
    {
        cout << i+1 <<". "<< songs[i] << endl ;
    }
}
/************************************************************************
*   functions and data structures from lecture #9 IPC031:
************************************************************************/
void print (int index)
{
    int stars = iteration/100000;
    cout << "After " << index << " element sorted:";

    while (stars > 0 )
    {
        cout <<"*";
        stars--;
    }
    cout<<"."<< endl;
}

struct Slice
{
	int from ; 	// 0    <= from
	int to ; 	// from <= to
} ;

Slice mkSlice (int from, int to)
{//	pre-condition:
	assert (0 <= from && from <= to);
/*  post-condition:
    result is the slice formed by from and to.
*/	Slice s = { from, to } ;
	return s ;
}

bool valid_slice (Slice s)
{// pre-condition:
    assert (true) ;
/*  post-condition:
    result is true only if s.from is not negative, and s.from is not larger than s.to.
*/
	return 0 <= s.from && s.from <= s.to ;
}

typedef vector<Track> El ;

bool is_sorted ( El& data, Slice s)
{
    //	pre-condition:
	assert (valid_slice(s)) ;	// ...and s.to < size of data
//	post-condition:
//	result is true if	data[s.from]   <= data[s.from+1]
//						data[s.from+1] <= data[s.from+2]
//						...
//						data[s.to-1]   <= data[s.to]
	bool sorted = true ;
	for (int i = s.from ; i < s.to && sorted; i++)
		if (data[i] > data[i+1])
			sorted = false ;
	return sorted ;
}

int find_position ( El& data, Slice s, Track y )
{
    //	pre-condition:
	assert (valid_slice(s) && is_sorted(data,s)) ;    // ...and s.to < size of data
    //	post-condition: if y <= to a an element in the slice it returns its position

	for ( int i = s.to ; i >= s.from ; i-- )
		if ( y > data [i] )
			return i + 1 ;
	return s.from ;
}

void shift_right ( El& data, Slice s )
{
    //	pre-condition:
	assert (valid_slice (s)) ;	// ... and s.to < size (data) - 1
//	post-condition:  data[s.from+1]	= 	old data[s.from]
//			    	 data[s.from+2]	= 	old data[s.from+1]
//						...
//			    	 data[s.to+1]		= 	old data[s.to]
	for ( int i = s.to+1 ; i > s.from ; i-- )
		data [i]  = data [i-1] ;
}

void insert ( El& data, int& length, Track y )
{
    // pre-condition:
    Slice s = mkSlice(0,length-1) ;
    assert (length >= 0 && is_sorted (data, s)) ;
  /*post-condition:
    value y is inserted in data[0] ... data[length-1] at the right ordering position,
    and length is increased by one.
*/
	const int POS = find_position (data, mkSlice (0, length), y) ;
	if (POS < length)
		shift_right ( data, mkSlice (POS, length-1) ) ;
	data [POS] = y ;
	length++;
}

void insertion_sort ( El& data, int length )
{
	int sorted = 1 ;
    while ( sorted < length )
    {
        insert ( data, sorted, data[sorted] ) ;
    }
}
/*
void insertion_sort ( El& data, int length )
{
	int sorted = 1 ;
    while ( sorted < length ) //huge loop from 1 to length - 1
    {
        Track y = data[sorted]; // consider all the elements from 1 to length - 1.
        Slice s = mkSlice (0, sorted) ; //from (0,0) to (0, length - 2).
        int POS ;

        for ( int i = s.to ; i >= s.from ; i-- ) //scroll into the sorted part
        {
            if ( y <= data [i] )
            {
                POS = i ; // if not the position is immediately after the sorted part.
            }
            else
            {
               POS = i + 1; //if we can find a lower element from the one we started with,we return its position
               break ;
            }

        }
	    if (POS < sorted) //if a minor element has been found..
        {
            for ( int i = sorted ; i > POS ; i-- ) //shift everything to the right
            {
                data [i]  = data [i-1] ;
            }
        }
	    data [POS] = y ; //the element is inserted in the right position
        sorted++; //length of the sorted is increased by one.
    }
}
*/
/************************************************************************
*   the main structure of the program:
*       - read file
*       - sort data with insertion_sort / selection_sort / bubble_sort
*              and 'normal' order of Tracks
*              and increasing track length
************************************************************************/
int main()
{
	const int NO_OF_SONGS = read_file ("Tracks.txt");
	if (NO_OF_SONGS < 0)
	{
        cout << "Reading file failed. Program terminates." << endl;
	    return NO_OF_SONGS;
    }

    for (int i = 99; i  <= NO_OF_SONGS; i += 100)
    {
        vector<Track> subvector(songs.begin(), songs.begin() + i);
        insertion_sort(subvector,i);
        cout << "N. of comparisons: " << iteration << endl;
        print(i);
        iteration = 0 ;
        if (i == 5799)
        i=NO_OF_SONGS-100;
    }

    //insertion_sort(songs,NO_OF_SONGS);
    cout << "Tracks sorted." << endl;
    //show_all_tracks (songs,NO_OF_SONGS) ;
	return 0;
}
/***********************************************************************************************************
 * DIFFERENCES BETWEEN THE ALGORITHMS:
 * sort by track:
 *  Insertion sort: 17.339.404
 *  New Insertion sort: 24.111
 *
 *
 *  Observation: the new Insertion algorithm is way more efficient, probably because the the vector is  already
 *               mostly ordered.
 *
 *
 * sort by time:
 *  Insertion sort: 8.616.400
 *  New Insertion sort: 8.750.238
 *
 *
 *  Observation: In this case the difference is not significant.
 ***********************************************************************************************************/
