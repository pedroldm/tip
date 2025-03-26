/**
 * api-usage.cpp
 *
 * Driver class with a simple example of how to instantiate and use the BRKGA API.
 * See SampleDecoder.h for details on the decoder's implementation.
 *
 * Created on : Nov 17, 2011 by rtoso
 * Authors    : Rodrigo Franco Toso <rtoso@cs.rutgers.edu>
 *              Mauricio G.C. Resende <mgcr@research.att.com>
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018
 * Rodrigo Franco Toso (rfrancotoso@gmail.com) and
 * Mauricio G.C. Resende
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <algorithm>
#include <iostream>

#include "SampleDecoder.hpp"
#include "brkgaAPI/BRKGA.h"
#include "brkgaAPI/MTRand.h"
#include "IOReport.hpp"

int main(int argc, char* argv[]) {
    IOReport ior(argc, argv);
    ior.run();
    ior.irace ? ior.bestFitness() : ior.printIOReport();
    return 0;
}