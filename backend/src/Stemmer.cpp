// Source : https://tartarus.org/martin/PorterStemmer/c_thread_safe.txt

/* This is the Porter stemming algorithm, coded up in ANSI C by the
author. It may be be regarded as canonical, in that it follows the
algorithm presented in

Porter, 1980, An algorithm for suffix stripping, Program, Vol. 14,
no. 3, pp 130-137,

only differing from it at the points marked --DEPARTURE-- below.

See also http://www.tartarus.org/~martin/PorterStemmer

The algorithm as described in the paper could be exactly replicated
by adjusting the points of DEPARTURE, but this is barely necessary,
because (a) the points of DEPARTURE are definitely improvements, and
(b) no encoding of the Porter stemmer I have seen is anything like
as exact as this version, even with the points of DEPARTURE!

You can compile it on Unix with 'gcc -O3 -o stem stem.c' after which
'stem' takes a list of inputs and sends the stemmed equivalent to
stdout.

The algorithm as encoded here is particularly fast.

Release 1: the basic non-thread safe version
Release 2: the thread-safe version
Release 3: 11 Apr 2013, fixes a bug noted by Matt Patenaude
Release 4: 25 Mar 2014, fixes a bug noted by Klemens Baum
*/


//#include <stdlib.h>  /* for malloc, free */
//#include <string.h>  /* for memcmp, memmove */
#include <cstring>


/* stemmer is a structure for a few local bits of data,
*/

class Stemmer {
public:
	int stem(char * b, int k);

private:
	char * b;       /* buffer for word to be stemmed */
	int k;          /* offset to the end of the string */
	int j;          /* a general offset into the string */

private:
	bool cons(int i);
	int m();
	bool vowelinstem();
	bool doublec(int j);
	bool cvc(int i);
	bool ends(char * s);
	void setto(char * s);
	void r(char * s);
	void step1ab();
	void step1c();
	void step2();
	void step3();
	void step4();
	void step5();
};


/* Member b is a buffer holding a word to be stemmed. The letters are in
b[0], b[1] ... ending at b[z->k]. Member k is readjusted downwards as
the stemming progresses. Zero termination is not in fact used in the
algorithm.

Note that only lower case sequences are stemmed. Forcing to lower case
should be done before stem(...) is called.


Typical usage is:

struct stemmer * z = create_stemmer();
char b[] = "pencils";
int res = stem(z, b, 6);
/- stem the 7 characters of b[0] to b[6]. The result, res,
will be 5 (the 's' is removed). -/
free_stemmer(z);
*/


/* The main part of the stemming algorithm starts here.
*/

/* cons(z, i) is TRUE <=> b[i] is a consonant.
*/

bool Stemmer::cons(int i)
{
	Stemmer* const z = this;

	switch (z->b[i])
	{
	case 'a': case 'e': case 'i': case 'o': case 'u': return false;
	case 'y': return (i == 0) ? true : !z->cons(i - 1);
	default: return true;
	}
}

/* m(z) measures the number of consonant sequences between 0 and j. if c is
a consonant sequence and v a vowel sequence, and <..> indicates arbitrary
presence,

<c><v>       gives 0
<c>vc<v>     gives 1
<c>vcvc<v>   gives 2
<c>vcvcvc<v> gives 3
....
*/

int Stemmer::m()
{
	Stemmer* const z = this;

	int n = 0;
	int i = 0;
	int j = z->j;
	for (;;)
	{
		if (i > j) return n;
		if (!z->cons(i)) break; i++;
	}
	i++;
	for (;;)
	{
		for (;;)
		{
			if (i > j) return n;
			if (z->cons(i)) break;
			i++;
		}
		i++;
		n++;
		for (;;)
		{
			if (i > j) return n;
			if (!z->cons(i)) break;
			i++;
		}
		i++;
	}
}

/* vowelinstem(z) is TRUE <=> 0,...j contains a vowel */

bool Stemmer::vowelinstem()
{
	Stemmer* const z = this;

	int j = z->j;
	int i; for (i = 0; i <= j; i++) if (!z->cons(i)) return true;
	return false;
}

/* doublec(z, j) is TRUE <=> j,(j-1) contain a double consonant. */

bool Stemmer::doublec(int j)
{
	Stemmer* const z = this;

	char * b = z->b;
	if (j < 1) return false;
	if (b[j] != b[j - 1]) return false;
	return z->cons(j);
}

/* cvc(z, i) is TRUE <=> i-2,i-1,i has the form consonant - vowel - consonant
and also if the second c is not w,x or y. this is used when trying to
restore an e at the end of a short word. e.g.

cav(e), lov(e), hop(e), crim(e), but
snow, box, tray.
*/

bool Stemmer::cvc(int i)
{
	Stemmer* const z = this;

	if (i < 2 || !z->cons(i) || z->cons(i - 1) || !z->cons(i - 2)) return false;
	{  int ch = z->b[i];
	if (ch == 'w' || ch == 'x' || ch == 'y') return false;
	}
	return true;
}

/* z->ends(s) is TRUE <=> 0,...k ends with the string s. */

bool Stemmer::ends( char * s)
{
	Stemmer* const z = this;

	int length = s[0];
	char * b = z->b;
	int k = z->k;
	if (s[length] != b[k]) return false; /* tiny speed-up */
	if (length > k + 1) return false;
	if (std::memcmp(b + k - length + 1, s + 1, length) != 0) return false;
	z->j = k - length;
	return true;
}

/* setto(z, s) sets (j+1),...k to the characters in the string s, readjusting
k. */

void Stemmer::setto(char * s)
{
	Stemmer* const z = this;

	int length = s[0];
	int j = z->j;
	std::memmove(z->b + j + 1, s + 1, length);
	z->k = j + length;
}

/* z->r(s) is used further down. */

void Stemmer::r(char * s) { Stemmer* const z = this; if (z->m() > 0) z->setto(s); }

/* step1ab(z) gets rid of plurals and -ed or -ing. e.g.

caresses  ->  caress
ponies    ->  poni
ties      ->  ti
caress    ->  caress
cats      ->  cat

feed      ->  feed
agreed    ->  agree
disabled  ->  disable

matting   ->  mat
mating    ->  mate
meeting   ->  meet
milling   ->  mill
messing   ->  mess

meetings  ->  meet

*/

void Stemmer::step1ab()
{
	Stemmer* const z = this;

	char * b = z->b;
	if (b[z->k] == 's')
	{
		if (z->ends("\04" "sses")) z->k -= 2; else
			if (z->ends("\03" "ies")) z->setto("\01" "i"); else
				if (b[z->k - 1] != 's') z->k--;
	}
	if (z->ends("\03" "eed")) { if (z->m() > 0) z->k--; }
	else
		if ((z->ends("\02" "ed") || z->ends("\03" "ing")) && z->vowelinstem())
		{
			z->k = z->j;
			if (z->ends("\02" "at")) z->setto("\03" "ate"); else
				if (z->ends("\02" "bl")) z->setto("\03" "ble"); else
					if (z->ends("\02" "iz")) z->setto("\03" "ize"); else
						if (z->doublec(z->k))
						{
							z->k--;
							{  int ch = b[z->k];
							if (ch == 'l' || ch == 's' || ch == 'z') z->k++;
							}
						}
						else if (z->m() == 1 && z->cvc(z->k)) z->setto("\01" "e");
		}
}

/* step1c(z) turns terminal y to i when there is another vowel in the stem. */

void Stemmer::step1c()
{
	Stemmer* const z = this;

	if (z->ends("\01" "y") && z->vowelinstem()) z->b[z->k] = 'i';
}


/* step2(z) maps double suffices to single ones. so -ization ( = -ize plus
-ation) maps to -ize etc. note that the string before the suffix must give
m(z) > 0. */

void Stemmer::step2() {
	Stemmer* const z = this;

	switch (z->b[z->k - 1])
	{
	case 'a': if (z->ends("\07" "ational")) { z->r("\03" "ate"); break; }
			  if (z->ends("\06" "tional")) { z->r("\04" "tion"); break; }
			  break;
	case 'c': if (z->ends("\04" "enci")) { z->r("\04" "ence"); break; }
			  if (z->ends("\04" "anci")) { z->r("\04" "ance"); break; }
			  break;
	case 'e': if (z->ends("\04" "izer")) { z->r("\03" "ize"); break; }
			  break;
	case 'l': if (z->ends("\03" "bli")) { z->r("\03" "ble"); break; } /*-DEPARTURE-*/

																	  /* To match the published algorithm, replace this line with
																	  case 'l': if (z->ends("\04" "abli")) { z->r("\04" "able"); break; } */

			  if (z->ends("\04" "alli")) { z->r("\02" "al"); break; }
			  if (z->ends("\05" "entli")) { z->r("\03" "ent"); break; }
			  if (z->ends("\03" "eli")) { z->r("\01" "e"); break; }
			  if (z->ends("\05" "ousli")) { z->r("\03" "ous"); break; }
			  break;
	case 'o': if (z->ends("\07" "ization")) { z->r("\03" "ize"); break; }
			  if (z->ends("\05" "ation")) { z->r("\03" "ate"); break; }
			  if (z->ends("\04" "ator")) { z->r("\03" "ate"); break; }
			  break;
	case 's': if (z->ends("\05" "alism")) { z->r("\02" "al"); break; }
			  if (z->ends("\07" "iveness")) { z->r("\03" "ive"); break; }
			  if (z->ends("\07" "fulness")) { z->r("\03" "ful"); break; }
			  if (z->ends("\07" "ousness")) { z->r("\03" "ous"); break; }
			  break;
	case 't': if (z->ends("\05" "aliti")) { z->r("\02" "al"); break; }
			  if (z->ends("\05" "iviti")) { z->r("\03" "ive"); break; }
			  if (z->ends("\06" "biliti")) { z->r("\03" "ble"); break; }
			  break;
	case 'g': if (z->ends("\04" "logi")) { z->r("\03" "log"); break; } /*-DEPARTURE-*/

																	   /* To match the published algorithm, delete this line */

	}
}

/* step3(z) deals with -ic-, -full, -ness etc. similar strategy to step2. */

void Stemmer::step3() {
	Stemmer* const z = this;

	switch (z->b[z->k])
	{
	case 'e': if (z->ends("\05" "icate")) { z->r("\02" "ic"); break; }
			  if (z->ends("\05" "ative")) { z->r("\00" ""); break; }
			  if (z->ends("\05" "alize")) { z->r("\02" "al"); break; }
			  break;
	case 'i': if (z->ends("\05" "iciti")) { z->r("\02" "ic"); break; }
			  break;
	case 'l': if (z->ends("\04" "ical")) { z->r("\02" "ic"); break; }
			  if (z->ends("\03" "ful")) { z->r("\00" ""); break; }
			  break;
	case 's': if (z->ends("\04" "ness")) { z->r("\00" ""); break; }
			  break;
	}
}

/* step4(z) takes off -ant, -ence etc., in context <c>vcvc<v>. */

void Stemmer::step4()
{
	Stemmer* const z = this;

	switch (z->b[z->k - 1])
	{
	case 'a': if (z->ends("\02" "al")) break; return;
	case 'c': if (z->ends("\04" "ance")) break;
		if (z->ends("\04" "ence")) break; return;
	case 'e': if (z->ends("\02" "er")) break; return;
	case 'i': if (z->ends("\02" "ic")) break; return;
	case 'l': if (z->ends("\04" "able")) break;
		if (z->ends("\04" "ible")) break; return;
	case 'n': if (z->ends("\03" "ant")) break;
		if (z->ends("\05" "ement")) break;
		if (z->ends("\04" "ment")) break;
		if (z->ends("\03" "ent")) break; return;
	case 'o': if (z->ends("\03" "ion") && z->j >= 0 && (z->b[z->j] == 's' || z->b[z->j] == 't')) break;
		if (z->ends("\02" "ou")) break; return;
		/* takes care of -ous */
	case 's': if (z->ends("\03" "ism")) break; return;
	case 't': if (z->ends("\03" "ate")) break;
		if (z->ends("\03" "iti")) break; return;
	case 'u': if (z->ends("\03" "ous")) break; return;
	case 'v': if (z->ends("\03" "ive")) break; return;
	case 'z': if (z->ends("\03" "ize")) break; return;
	default: return;
	}
	if (z->m() > 1) z->k = z->j;
}

/* step5(z) removes a final -e if m(z) > 1, and changes -ll to -l if
m(z) > 1. */

void Stemmer::step5()
{
	Stemmer* const z = this;

	char * b = z->b;
	z->j = z->k;
	if (b[z->k] == 'e')
	{
		int a = z->m();
		if (a > 1 || ( a == 1 && !z->cvc(z->k - 1) ) ) z->k--;
	}
	if (b[z->k] == 'l' && z->doublec(z->k) && z->m() > 1) z->k--;
}

/* In stem(z, b, k), b is a char pointer, and the string to be stemmed is
from b[0] to b[k] inclusive.  Possibly b[k+1] == '\0', but it is not
important. The stemmer adjusts the characters b[0] ... b[k] and returns
the new end-point of the string, k'. Stemming never increases word
length, so 0 <= k' <= k.
*/

int Stemmer::stem(char * b, int k)
{
	Stemmer* const z = this;

	if (k <= 1) return k; /*-DEPARTURE-*/
	z->b = b; z->k = k; /* copy the parameters into z */

						/* With this line, strings of length 1 or 2 don't go through the
						stemming process, although no mention is made of this in the
						published algorithm. Remove the line to match the published
						algorithm. */

	z->step1ab();
	if (z->k > 0) {
		z->step1c(); z->step2(); z->step3(); z->step4(); z->step5();
	}
	return z->k;
}

/*--------------------stemmer definition ends here------------------------*/

//#include <stdio.h>
//#include <ctype.h>       // for isupper, islower, tolower //
/*
#define INC 50           // size units in which s is increased //

#define LETTER(ch) (isupper(ch) || islower(ch))

template <typename type>
type* realloc(type* ptr, size_t oldsize, size_t newsize)
{
	type* newptr = new type[newsize];
	std::memcpy(newptr, ptr, oldsize < newsize ? oldsize : newsize);
	delete[] ptr;
	return newptr;
}

void stemfile(Stemmer * z, FILE * f)
{
	char * s;         // buffer for words to be stemmed //
	int i_max = INC;  // maximum offset in s //

	s = new char[i_max + 1];

	for (;;)
	{
		int ch = getc(f);
		if (ch == EOF) return;
		if (LETTER(ch))
		{
			int i = 0;
			for (;;)
			{
				if (i == i_max)
				{
					s = realloc(s, i_max, i_max + INC + 1);
					i_max += INC;
				}
				ch = tolower(ch); // forces lower case //

				s[i] = ch; i++;
				ch = getc(f);
				if (!LETTER(ch)) { ungetc(ch, f); break; }
			}
			s[z->stem(s, i - 1) + 1] = 0;
			// the previous line calls the stemmer and //
			// uses its result to zero-terminate the string in s //
			printf("%s", s);
		}
		else putchar(ch);
	}

	delete[] s;
}
*/
/*
int main(int argc, char * argv[])
{
	int i;

	Stemmer z;

	for (i = 1; i < argc; i++)
	{
		FILE * f = fopen(argv[i], "r");
		if (f == 0) { fprintf(stderr, "File %s not found\n", argv[i]); exit(1); }
		stemfile(&z, f);
	}

	return 0;
}
*/

void stem(char* word)
{
	int length = std::strlen(word);
	if (length > 0)
	{
		Stemmer s;
		word[s.stem(word, length - 1) + 1] = '\0';
	}
}
