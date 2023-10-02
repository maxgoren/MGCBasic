/*
MIT License

Copyright (c) 2023 Max Goren, http://www.maxgcoding.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <iostream>
#include <vector>
#include "include/mgcbasic_interpreter.hpp"
#include "include/mgcbasic_globals.hpp"
using namespace std;

void banner() {
    string m_banner = m_NAME + " " + m_VERSION + " (c) 2023 maxgcoding.com";
    cout<<m_banner<<endl;
    for (int i = 0; i <= m_banner.length(); i++) cout<<"=";
    cout<<endl;
}

int main(int argc, char* argv[]) {
    MGCBasic mgc;
    if (argc < 2) {
        banner();
        mgc.REPL();
    } else {
        string filename = argv[1];
        mgc.runFromFile(filename);
    }
    return 0;
}