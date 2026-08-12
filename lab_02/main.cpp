#include <bitset>

#include "des.hpp"
#include "cfb.hpp"

using namespace std;

int main() {
    constexpr bitset64 key(0x4141414141414141);
    constexpr bitset64 IV(0x4128947691623128);

    // -- Text files --
    CBF().encryptFile<64>(
        "/Users/stepa/Study/InfoSec/lab_02/data/in.txt",
        "/Users/stepa/Study/InfoSec/lab_02/data/enc.txt",
        IV,
        key,
        't'
    );
    CBF().decryptFile<64>(
        "/Users/stepa/Study/InfoSec/lab_02/data/enc.txt",
        "/Users/stepa/Study/InfoSec/lab_02/data/decr.txt",
        IV,
        key,
        't'
    );
    // -- Image files --
    CBF().encryptFile<16>(
        "/Users/stepa/Study/InfoSec/lab_02/data/in.jpg",
        "/Users/stepa/Study/InfoSec/lab_02/data/enc.jpg",
        IV,
        key,
        'b'
    );
    CBF().decryptFile<16>(
        "/Users/stepa/Study/InfoSec/lab_02/data/enc.jpg",
        "/Users/stepa/Study/InfoSec/lab_02/data/decr.jpg",
        IV,
        key,
        'b'
    );
    // -- Archive files --
    CBF().encryptFile<8>(
        "/Users/stepa/Study/InfoSec/lab_02/data/in.zip",
        "/Users/stepa/Study/InfoSec/lab_02/data/enc.zip",
        IV,
        key,
        'a'
    );
    CBF().decryptFile<8>(
        "/Users/stepa/Study/InfoSec/lab_02/data/enc.zip",
        "/Users/stepa/Study/InfoSec/lab_02/data/decr.zip",
        IV,
        key,
        'a'
    );

    // -- MAN --
    // const vector<bitset<64>> blocks {
    //     bitset<64>(0x4141414141414141),
    //     bitset<64>(0x4141414141414141)
    // };
    //
    // const auto ok = CBF().encrypt(blocks, key, IV);
    // for (auto block: ok) {
    //     cout << hex << block.to_ullong() << endl;
    // }
    // const auto ko = CBF().decrypt(ok, key, IV);
    // for (auto block: ko) {
    //     cout << hex << block.to_ullong() << endl;
    // }

    return 0;
}
