#ifndef GENESIS_H
#define GENESIS_H

#include "bignum.h"

// Genesis block 0 hash
static const uint256 hashGenesisBlock("00000b17e3a241903ee4b5be429ce0d3f5097312a7b9ab093cc172aece0943a5");
static const uint256 hashGenesisBlockTestNet("00000b17e3a241903ee4b5be429ce0d3f5097312a7b9ab093cc172aece0943a5");

// Chechpoints to ensure the correct chain usage
static const uint256 CheckBlock1 ("0000000000de57f302cf24938087273e419e1204913bd8683697e68be143f818"); // Checkpoint at block 1000
static const uint256 CheckBlock2 ("0000000001e7721237843466908080148eda23e67f70fca89faccf8c017f99da"); // Checkpoint at block 6500
static const uint256 CheckBlock3 ("00000000023c10d7add3836461460365d5e32b44ba86b1994e888480fdb72515"); // Checkpoint at block 10000
static const uint256 CheckBlock4 ("000000000041645cf15e7a22384b0bdc9ba1f46136449c1675f9554a11a1f876"); // Checkpoint at block 21001
static const uint256 CheckBlock5 ("248b7e38881da014baef9043acff857a18221fe07827ecd06d055669f5f521a6"); // Checkpoint at block 44000
static const uint256 CheckBlock6 ("12cc1ec8a0ba25ca6db18283145e9bcde6e250ef281130dffa87ad1ffbf01475"); // Checkpoint at block 63000
static const uint256 CheckBlock7 ("1d9129bb22d7eec87a71607fcc19dddda45d359fb243e2957d81d5f5127fbb8c"); // Checkpoint at block 73000
static const uint256 CheckBlock8 ("75c03ef922d50f66c255bc72aee2d4a57bf6359a34e3213bcaf34adf4a24435a"); // Checkpoint at block 80000
static const uint256 CheckBlock9 ("af2c40c1479633c9180ff965a01f3f7a4d5c7e161456c5ca427c77a2c8c65c99"); // Checkpoint at block 210900

// TODO: Move Kernel checksum in here


#endif // GENESIS_H
