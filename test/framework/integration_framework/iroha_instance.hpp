/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_IROHA_INSTANCE_HPP
#define IROHA_IROHA_INSTANCE_HPP

#include <boost/optional.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>
#include <memory>
#include <string>
#include "ametsuchi/impl/postgres_options.hpp"
#include "multi_sig_transactions/gossip_propagation_strategy_params.hpp"

namespace shared_model {
  namespace interface {
    class Block;
  }  // namespace interface
  namespace crypto {
    class Keypair;
  }  // namespace crypto
}  // namespace shared_model

namespace integration_framework {
  class TestIrohad;

  class IrohaInstance {
   public:
    /**
     * @param mst_support enables multisignature tx support
     * @param block_store_path
     * @param listen_ip - ip address for opening ports (internal & torii)
     * @param torii_port - port to bind Torii service to
     * @param internal_port - port for internal irohad communication
     * @param dbname is a name of postgres database
     */
    IrohaInstance(bool mst_support,
                  const std::string &block_store_path,
                  const std::string &listen_ip,
                  size_t torii_port,
                  size_t internal_port,
                  const boost::optional<std::string> &dbname = boost::none);

    void makeGenesis(const shared_model::interface::Block &block);

    void rawInsertBlock(const shared_model::interface::Block &block);

    void setMstGossipParams(
        std::chrono::milliseconds mst_gossip_emitting_period,
        uint32_t mst_gossip_amount_per_once);

    void initPipeline(const shared_model::crypto::Keypair &key_pair,
                      size_t max_proposal_size = 10);

    void run();

    std::shared_ptr<TestIrohad> &getIrohaInstance();

    static std::string getPostgreCredsOrDefault(
        const boost::optional<std::string> &dbname);

    // config area
    const std::string block_store_dir_;
    const std::string pg_conn_;
    const std::string listen_ip_;
    const size_t torii_port_;
    const size_t internal_port_;
    const std::chrono::milliseconds proposal_delay_;
    const std::chrono::milliseconds vote_delay_;
    const std::chrono::minutes mst_expiration_time_;
    boost::optional<iroha::GossipPropagationStrategyParams>
        opt_mst_gossip_params_;

   private:
    std::shared_ptr<TestIrohad> instance_;

    boost::optional<std::chrono::milliseconds> mst_gossip_emitting_period_;
    boost::optional<uint32_t> mst_gossip_amount_per_once_;
  };
}  // namespace integration_framework
#endif  // IROHA_IROHA_INSTANCE_HPP
