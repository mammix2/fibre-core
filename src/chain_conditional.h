#ifndef CHAIN_CONDITIONAL_H
#define CHAIN_CONDITIONAL_H

extern bool fTestNet;

static const int P1_End = 43200; // V1 Blockchain PoW end.
static const int P2_Start = 673686; // V2 Blockchain PoW start
static const int P2_End = 10000000; // 0 coin reward PoW mining enabled for about 5 years. Incase of PoS diff drop


static const int P1_End_TestNet = 150;
static const int P2_Start_TestNet = 160;
static const int P2_End_TestNet = 200; // Phase 2 PoW start


#endif // CHAIN_CONDITIONAL_H
