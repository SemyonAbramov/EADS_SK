#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits.h>

using namespace std;
#define INFINITY INT_MAX

typedef struct optimal_format {
    unsigned min_cost;
    unsigned num_of_lines;
} optimal_format_t;

int count_lines(std::vector<int> nlines, int n)
{
    if (nlines[n] == 1)
        return 1;

    unsigned num_of_lines = 1;
    int count = nlines[n] - 1;

    while (count != 0) {
        
        num_of_lines++;

        if (nlines[count] == 1) {
            return num_of_lines;
        }
    
        count  = nlines[count] - 1;
    }

    return num_of_lines;
}

void construct_cost_table(vector< vector<int> >* cost_table, std::vector<int> words_len, int L)
{
    int n = words_len.size();
    std::vector< vector<int> > right_margins(n + 1, vector<int> (n + 1, 0));

    for (int i = 1; i <= n; i++) {
        right_margins[i][i] = L -  words_len[i-1];
        
        for (int j = i + 1; j <= n; j++)  {
            right_margins[i][j] = right_margins[i][j-1] - words_len[j-1] - 1;
        }
    }

    for (int i = 1; i <= n; i++) {
        for (int j = i; j <= n; j++) {
            if (right_margins[i][j] >= 0)
                (*cost_table)[i][j] = right_margins[i][j] * right_margins[i][j];
            else
                (*cost_table)[i][j] = INFINITY;
        }
    }
}

optimal_format_t* format_text(std::vector<int> words_len, int L)
{
    int n = words_len.size();
    std::vector< vector<int> > cost_table(n + 1, vector<int> (n + 1, 0));
    
    construct_cost_table(&cost_table, words_len, L);

    std::vector<int> costs (n + 1, INFINITY);
    costs[0] = 0;
    
    std::vector<int> nwords(n + 1);
    for (int j = 1; j <= n; j++) {
        for (int i = 1; i <= j; i++) {
            if ((cost_table[i][j] != INFINITY) && (costs[i-1] != INFINITY) && (costs[i-1] + cost_table[i][j] < costs[j])) {
                nwords[j] = i;
                costs[j] = costs[i-1] + cost_table[i][j];
            }
        }
    }

    unsigned num_of_lines = 0;
    num_of_lines = count_lines(nwords, n);

    optimal_format_t* opt_format = new optimal_format_t();
    opt_format->min_cost = costs[n];
    opt_format->num_of_lines = num_of_lines;

    return opt_format;
}

int main()
{
    std::ifstream infile("input.txt");
    
    int L = 0;
    infile >> L;
    
    std::vector<int> words_len;
    
    int num_of_words = 0;    
    for (std::string word; infile >> word; ++num_of_words) {
        words_len.push_back(word.length());
    }

    optimal_format_t* opt_format = format_text(words_len, L);
    
    ofstream outfile;
    outfile.open ("output.txt");

    outfile << opt_format->min_cost << endl;
    outfile << opt_format->num_of_lines;

    infile.close();
    outfile.close();

    delete opt_format;

    return 0; 
}
