#ifndef GDK_CLIENT_BLOB_HPP
#define GDK_CLIENT_BLOB_HPP
#pragma once

#include <cstdint>
#include <deque>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <optional>

#include "ga_wally.hpp"
#include <nlohmann/json.hpp>

namespace green {

    // Client-only data, stored on a server as a server-unreadable blob
    class client_blob final {
    public:
        client_blob();
        client_blob(const client_blob&) = delete;
        client_blob& operator=(const client_blob&) = delete;
        client_blob(client_blob&&) = delete;
        client_blob& operator=(client_blob&&) = delete;

        void reset();

        // Set the encryption key
        void set_key(pbkdf2_hmac256_t key);
        // Compute the encryption and HMAC keys from a privately derived public key
        void compute_keys(byte_span_t public_key);

        bool has_key() const;
        pbkdf2_hmac256_t get_key() const;
        bool has_hmac_key() const;

        void set_user_version(uint64_t version);
        uint64_t get_user_version() const;

        bool update_subaccounts_data(const nlohmann::json& subaccounts, const nlohmann::json& xpubs);
        nlohmann::json get_subaccounts_data() const;
        nlohmann::json get_subaccount_data(uint32_t subaccount) const;

        bool set_tx_memo(const std::string& txhash_hex, const std::string& memo);
        bool update_tx_memos(const nlohmann::json& memos);
        std::string get_tx_memo(const std::string& txhash_hex) const;
        nlohmann::json get_tx_memos() const;

        bool set_master_blinding_key(const std::string& master_blinding_key_hex);
        std::string get_master_blinding_key() const;
        bool is_master_blinding_key_denied() const;

        bool set_wo_data(const std::string& username, const nlohmann::json& xpubs);
        bool set_xpubs(const nlohmann::json& xpubs);
        std::string get_wo_username() const;
        nlohmann::json get_xpubs() const;

        void load(byte_span_t data, bool merge_current = false);
        std::pair<std::vector<unsigned char>, nlohmann::json> save() const;

        static const std::string& get_zero_hmac();
        static const std::string& get_one_hmac();
        std::string compute_hmac(byte_span_t data) const;

    private:
        bool is_key_encrypted(uint32_t key) const;

        bool merge_xpubs(const nlohmann::json& xpubs);

        nlohmann::json m_data;
        // Key for encrypting the client blob contents
        std::optional<pbkdf2_hmac256_t> m_key;
        // Key for generating blob HMAC. Only set if the
        // client blob is writable.
        std::optional<pbkdf2_hmac256_t> m_hmac_key;
    };

} // namespace green

#endif
