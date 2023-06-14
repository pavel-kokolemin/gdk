#ifndef GDK_GA_TX_HPP
#define GDK_GA_TX_HPP
#pragma once

#include "ga_wally.hpp"
#include <nlohmann/json.hpp>

namespace ga {
namespace sdk {
    class network_parameters;
    class session_impl;

    class Tx {
    public:
        Tx(uint32_t locktime, uint32_t version, bool is_liquid);
        Tx(byte_span_t tx_bin, bool is_liquid);
        Tx(const std::string& tx_hex, bool is_liquid);
        Tx(const wally_psbt_ptr& psbt);

        Tx(Tx&& rhs) = default;
        Tx(Tx& rhs) = delete;
        Tx(const Tx& rhs) = delete;

        void swap(Tx& rhs);

        std::vector<unsigned char> to_bytes() const;
        std::string to_hex() const;

        size_t get_num_inputs() const { return m_tx->num_inputs; }
        struct wally_tx_input& get_input(size_t index);
        const struct wally_tx_input& get_input(size_t index) const;
        auto get_inputs() { return gsl::make_span(m_tx->inputs, m_tx->num_inputs); }
        auto get_inputs() const { return gsl::make_span(m_tx->inputs, m_tx->num_inputs); }

        void add_input(byte_span_t txhash, uint32_t index, uint32_t sequence, byte_span_t script,
            const wally_tx_witness_stack_ptr& witness = {});

        void set_input_script(size_t index, byte_span_t script);

        void set_input_witness(size_t index, const wally_tx_witness_stack_ptr& witness);

        size_t get_num_outputs() const { return m_tx->num_outputs; }

        struct wally_tx_output& get_output(size_t index);
        const struct wally_tx_output& get_output(size_t index) const;
        auto get_outputs() { return gsl::make_span(m_tx->outputs, m_tx->num_outputs); }
        auto get_outputs() const { return gsl::make_span(m_tx->outputs, m_tx->num_outputs); }

        void add_output(uint64_t satoshi, byte_span_t script);
        void add_elements_output_at(size_t index, byte_span_t script, byte_span_t asset, byte_span_t value,
            byte_span_t nonce, byte_span_t surjectionproof, byte_span_t rangeproof);

        void set_output_commitments(size_t index, byte_span_t asset, byte_span_t value, byte_span_t nonce,
            byte_span_t surjectionproof, byte_span_t rangeproof);
        void set_output_satoshi(size_t index, const std::string& asset_id, uint64_t satoshi);

        uint32_t get_version() const { return m_tx->version; }
        uint32_t get_locktime() const { return m_tx->locktime; }
        void set_anti_snipe_locktime(uint32_t current_block_height);

        size_t get_weight() const;
        static size_t vsize_from_weight(size_t weight);
        size_t get_adjusted_weight(const network_parameters& net_params) const;
        uint64_t get_fee(const network_parameters& net_params, uint64_t fee_rate) const;

        std::array<unsigned char, SHA256_LEN> get_btc_signature_hash(
            size_t index, byte_span_t script, uint64_t satoshi, uint32_t sighash, uint32_t flags) const;

        std::array<unsigned char, SHA256_LEN> get_elements_signature_hash(
            size_t index, byte_span_t script, byte_span_t value, uint32_t sighash, uint32_t flags) const;

    private:
        uint32_t get_flags() const;
        struct tx_deleter {
            void operator()(struct wally_tx* p);
        };
        std::unique_ptr<struct wally_tx, tx_deleter> m_tx;
        bool m_is_liquid;
    };

    void utxo_add_paths(session_impl& session, nlohmann::json& utxo);

    std::array<unsigned char, SHA256_LEN> get_script_hash(
        const network_parameters& net_params, const nlohmann::json& utxo, const Tx& tx, size_t index, uint32_t sighash);

    nlohmann::json get_blinding_factors(const blinding_key_t& master_blinding_key, const nlohmann::json& details);

    void confidentialize_address(
        const network_parameters& net_params, nlohmann::json& addr, const std::string& blinding_pubkey_hex);

    void create_ga_transaction(session_impl& session, nlohmann::json& details);

    void add_input_signature(
        Tx& tx, uint32_t index, const nlohmann::json& u, const std::string& der_hex, bool is_low_r);

    std::vector<sig_and_sighash_t> get_signatures_from_input(
        const nlohmann::json& utxo, const Tx& tx, size_t index, bool is_liquid);

    nlohmann::json unblind_output(session_impl& session, const Tx& tx, uint32_t vout);

    std::vector<nlohmann::json> get_ga_signing_inputs(const nlohmann::json& details);

    std::pair<std::vector<std::string>, Tx> sign_ga_transaction(
        session_impl& session, const nlohmann::json& details, const std::vector<nlohmann::json>& inputs);
    nlohmann::json sign_ga_transaction(session_impl& session, const nlohmann::json& details);

    void blind_ga_transaction(session_impl& session, nlohmann::json& details, const nlohmann::json& blinding_data);

} // namespace sdk
} // namespace ga

#endif
