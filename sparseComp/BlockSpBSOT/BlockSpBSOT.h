#pragma once

#include "../Common/Common.h"
#include "../Common/VecMatrix.h"
#include "../Common/ZN.h"
#include "../CustomOPRF/CustomizedOPRF.h"
#include "coproto/Socket/Socket.h"
#include "cryptoTools/Crypto/PRNG.h"
#include <vector>
#include <cstdint>
#include <array>

#define RSOPRF_BIN_SIZE (1 << 14)

using point = sparse_comp::point;
using osuCrypto::PRNG;
using AES = osuCrypto::AES;
using block = osuCrypto::block;
using CustomOPRFSender = sparse_comp::custom_oprf::Sender;
using CustomOPRFReceiver = sparse_comp::custom_oprf::Receiver;

template<typename T, size_t N>
using array = std::array<T, N>;

template<typename T>
using vec = std::vector<T>;

namespace sparse_comp::block_sp_bsot {

    template<size_t tr, size_t t, size_t k, size_t n>
    class Sender {
            
        private:
            CustomOPRFSender* oprfSender;
            CustomOPRFReceiver* oprfReceiver;
            PRNG* prng;
            AES aes = AES(block(13133210048402866,17132091720387928));
            

        public:
            Sender(PRNG& prng, CustomOPRFSender* sender, CustomOPRFReceiver* receiver) {
                this->prng = &prng;
                this->oprfSender = sender;
                this->oprfReceiver = receiver;
            }

            ~Sender() {
                delete oprfSender;
                delete oprfReceiver;
            }

            Proto send(coproto::Socket& sock, vec<block>& ordIndexSet, array<array<array<block,n>,k>,t>& msg_vecs, array<array<ZN<n>,k>,t>& choice_vec_shares, array<array<block,k>,t>& output_shares);
    };

    template<size_t ts, size_t t, size_t k, size_t n>
    class Receiver {

        private:
            CustomOPRFReceiver* oprfReceiver;
            CustomOPRFSender* oprfSender;
            AES aes = AES(block(13133210048402866,17132091720387928));
            // void internalReceive(vector<block>& oprf_vals, vector<block>& paxos_structure, const array<point,t>& ordIndexSet, array<array<ZN<n>,k>,t>& choice_vec_shares, array<array<block,k>,t>& output_shares);

        public:
            Receiver(PRNG& prng, CustomOPRFReceiver* receiver, CustomOPRFSender* sender) {
                this->oprfSender = sender;
                this->oprfReceiver = receiver;
            }

            ~Receiver() {
                delete oprfReceiver;
                delete oprfSender;
            }

            Proto receive(coproto::Socket& sock, vec<block>& ordIndexSet, array<array<ZN<n>,k>,t>& choice_vec_shares, array<array<block,k>,t>& output_shares);
            
    };

};

#include "BlockSpBSOT.cpp"
