// -*-Mode: C++;-*-

//*BeginLicense**************************************************************
//
//---------------------------------------------------------------------------
// TAZeR (github.com/pnnl/tazer/)
//---------------------------------------------------------------------------
//
// Copyright ((c)) 2019, Battelle Memorial Institute
//
// 1. Battelle Memorial Institute (hereinafter Battelle) hereby grants
//    permission to any person or entity lawfully obtaining a copy of
//    this software and associated documentation files (hereinafter "the
//    Software") to redistribute and use the Software in source and
//    binary forms, with or without modification.  Such person or entity
//    may use, copy, modify, merge, publish, distribute, sublicense,
//    and/or sell copies of the Software, and may permit others to do
//    so, subject to the following conditions:
//    
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimers.
//
//    * Redistributions in binary form must reproduce the above
//      copyright notice, this list of conditions and the following
//      disclaimer in the documentation and/or other materials provided
//      with the distribution.
//
//    * Other than as used herein, neither the name Battelle Memorial
//      Institute or Battelle may be used in any form whatsoever without
//      the express written consent of Battelle.
//
// 2. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
//    CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
//    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
//    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//    DISCLAIMED. IN NO EVENT SHALL BATTELLE OR CONTRIBUTORS BE LIABLE
//    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
//    OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//    BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
//    USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//    DAMAGE.
//
// ***
//
// This material was prepared as an account of work sponsored by an
// agency of the United States Government.  Neither the United States
// Government nor the United States Department of Energy, nor Battelle,
// nor any of their employees, nor any jurisdiction or organization that
// has cooperated in the development of these materials, makes any
// warranty, express or implied, or assumes any legal liability or
// responsibility for the accuracy, completeness, or usefulness or any
// information, apparatus, product, software, or process disclosed, or
// represents that its use would not infringe privately owned rights.
//
// Reference herein to any specific commercial product, process, or
// service by trade name, trademark, manufacturer, or otherwise does not
// necessarily constitute or imply its endorsement, recommendation, or
// favoring by the United States Government or any agency thereof, or
// Battelle Memorial Institute. The views and opinions of authors
// expressed herein do not necessarily state or reflect those of the
// United States Government or any agency thereof.
//
//                PACIFIC NORTHWEST NATIONAL LABORATORY
//                             operated by
//                               BATTELLE
//                               for the
//                  UNITED STATES DEPARTMENT OF ENERGY
//                   under Contract DE-AC05-76RL01830
// 
//*EndLicense****************************************************************

#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: ./OpenTest /path/to/file" << std::endl;
        return 1;
    }

    std::string *fileName = new std::string(argv[1]);
    int fd = open(fileName->c_str(), O_RDONLY);
    if (fd == -1) {
        std::cout << "Failed to open " << fileName->c_str() << std::endl;
        return 1;
    }
    std::cout << "Opened: " << fileName->c_str();

    struct stat64 sb;
    if (lstat64(fileName->c_str(), &sb) == -1) {
        std::cout << "Failed open lstat: " << fileName->c_str() << std::endl;
        return 1;
    }
    std::cout << "File size: " << sb.st_size << std::endl;

    off64_t size = (off64_t)sb.st_size;
    off64_t off = lseek64(fd, size / 2, SEEK_SET);
    if (off != size / 2) {
        std::cout << "Failed SEEK_SET: " << off << std::endl;
        return 1;
    }
    std::cout << "SEEK_SET: " << off << std::endl;

    off = lseek64(fd, size / 4, SEEK_CUR);
    if (off != size * 3 / 4) {
        std::cout << "Failed SEEK_CUR: " << off << std::endl;
        return 1;
    }
    std::cout << "SEEK_CUR: " << off << std::endl;

    off = lseek64(fd, 0, SEEK_END);
    if (off != size) {
        std::cout << "Failed SEEK_END: " << off << std::endl;
        return 1;
    }
    std::cout << "SEEK_END: " << off << std::endl;

    off = lseek64(fd, 32, SEEK_END);
    if (off != size + 32) {
        std::cout << "Failed SEEK_END: " << off << std::endl;
        return 1;
    }
    std::cout << "SEEK_END: " << off << std::endl;

    if (close(fd) == -1) {
        std::cout << "Failed to close " << fileName->c_str() << std::endl;
        return 1;
    }
    std::cout << "Closed: " << fileName->c_str();

    return 0;
}
