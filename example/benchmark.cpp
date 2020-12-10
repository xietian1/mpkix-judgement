//
// Created by xt on 12/9/20.
//

//aka Levenshtein distance

#include "edit_distance_common.hpp"
// get the edit_alignment() function
#include <boost/algorithm/sequence/edit_distance.hpp>
using boost::algorithm::sequence::edit_distance;
using namespace boost::algorithm::sequence::parameter;
using boost::algorithm::sequence::unit_cost;

#include <stdio.h>
#include <string>
#include <stdint.h>
//OPE library
#include "sope_encode.h"

using namespace std;
using namespace sope;

struct EncodedTuple {
    size_t  len;
    uint8_t tuple[64];     // for simplicity

    EncodedTuple(int ii, const char* str, double dd) {
        *reinterpret_cast<uint32_t*>(tuple) = encode(ii);
        len = sizeof(uint32_t);
        size_t slen = strlen(str);
        uint32_t eslen = encode(str, slen, (void*)(tuple+len));
        len += eslen;
        *reinterpret_cast<uint64_t*>(tuple+len) = encode(dd);
        len += sizeof(uint64_t);
    }

    ~EncodedTuple() { }
};

#define MIN(x, y) ((x <= y) ? x : y)

int compTuple(const EncodedTuple & t1, const EncodedTuple & t2) {
    int rc = memcmp(t1.tuple, t2.tuple, MIN(t1.len,t2.len));
    if (rc == 0) {
        rc = (t1.len < t2.len) ? -1 : ( (t1.len > t2.len) ? 1 : 0 );
    } else {
        rc = (rc < 0) ? -1 : 1;
    }
    return rc;
}


int main(int argc, char** argv) {
    char const* str1_user = "Oh, hello world.";
    char const* str1_id = "Hello world!!";



    //stringstream_tuple_output<unit_cost, char const*> out;
    //unsigned dist = edit_distance(str1_user, str1_id, _script = out);
    //std::cout << "dist= " << dist << "   edit operations= " << out.ss.str() << "\n";

    unsigned dist = edit_distance(str1_user, str1_id);
    cout << "dist=" << dist;

    EncodedTuple tuple1(10, "This is a string", 1234.5678),
            tuple2(-10, "This is a string", 12345.6789),
            tuple3(100, "This is a string", 1234.5678),
            tuple4(10, "This is a string1", 1234.5678),
            tuple5(10, "This is a strin", 1234.5678),
            tuple6(10, "This is a string", -1234.5678),
            tuple7(10, "This is a string", 1234.5678);

    printf("Expected results:\n");
    printf("1 -1 -1 1 1 0\n");

    int rc = compTuple(tuple1, tuple2);
    printf("%d ", rc);

    rc = compTuple(tuple1, tuple3);
    printf("%d ", rc);

    rc = compTuple(tuple1, tuple4);
    printf("%d ", rc);

    rc = compTuple(tuple1, tuple5);
    printf("%d ", rc);

    rc = compTuple(tuple1, tuple6);
    printf("%d ", rc);

    rc = compTuple(tuple1, tuple7);
    printf("%d\n", rc);


    return 0;
}