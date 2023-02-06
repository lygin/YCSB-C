#ifndef YCSB_C_SIMPLEKV_H_
#define YCSB_C_SIMPLEKV_H_

#include "core/db.h"

#include <iostream>
#include <string>
#include <mutex>
#include "core/properties.h"
#include "simplekv_client.h"

namespace ycsbc {

class SimpleKV : public DB {
 public:
  SimpleKV() : kvs_("127.0.0.1", 8) {}

  int Read(const std::string &table, const std::string &key,
           const std::vector<std::string> *fields,
           std::vector<KVPair> &result) {
            
    std::lock_guard<std::mutex> lock(mutex_);
    if(simplekv_.find(key) == simplekv_.end()) {
      return kErrorNoData;
    }
    FieldKV *fieldkv = simplekv_.at(key);
    result.clear();
    if(!fields) {
      for(const KVPair &kv : *fieldkv) {
        result.emplace_back(kv);
      }
    } else {
      for(const std::string &field : *fields) {
        result.emplace_back(field, (*fieldkv)[field]);
      }
    }
    return 0;
  }

  int Scan(const std::string &table, const std::string &key,
           int len, const std::vector<std::string> *fields,
           std::vector<std::vector<KVPair>> &result) {
    std::lock_guard<std::mutex> lock(mutex_);
    cout << "SCAN " << table << ' ' << key << " " << len;
    if (fields) {
      cout << " [ ";
      for (auto f : *fields) {
        cout << f << ' ';
      }
      cout << ']' << endl;
    } else {
      cout  << " < all fields >" << endl;
    }
    return 0;
  }

  int Update(const std::string &table, const std::string &key,
             std::vector<KVPair> &values) {
    std::lock_guard<std::mutex> lock(mutex_);
    if(simplekv_.find(key) == simplekv_.end()) {
      FieldKV *newFieldKV = new FieldKV;
      simplekv_[key] = newFieldKV;
    }
    FieldKV *fieldkv = simplekv_.at(key);
    for(const KVPair &filekv : values) {
      fieldkv->emplace(filekv.first, filekv.second);
      kvs_.put(key+filekv.first, filekv.second);
    }
    return 0;
  }

  int Insert(const std::string &table, const std::string &key,
             std::vector<KVPair> &values) {
    std::lock_guard<std::mutex> lock(mutex_);
    if(simplekv_.find(key) == simplekv_.end()) {
      FieldKV *newFieldKV = new FieldKV;
      simplekv_[key] = newFieldKV;
    }
    FieldKV *fieldkv = simplekv_.at(key);
    for(const KVPair &filekv : values) {
      fieldkv->emplace(filekv.first, filekv.second);
      kvs_.put(key+filekv.first, filekv.second);
    }
    return 0;
  }

  int Delete(const std::string &table, const std::string &key) {
    std::lock_guard<std::mutex> lock(mutex_);
    if(simplekv_.find(key) == simplekv_.end()) {
      return 0;
    }
    delete simplekv_[key];
    simplekv_.erase(key);
    kvs_.del(key);
    return 0; 
  }
  using FieldKV = std::unordered_map<std::string, std::string>;
 private:
  std::mutex mutex_;
  std::unordered_map<std::string, FieldKV *> simplekv_;
  SimpleKVClient kvs_;
};

} // ycsbc

#endif // YCSB_C_BASIC_DB_H_

