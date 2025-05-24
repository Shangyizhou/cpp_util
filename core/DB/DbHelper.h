//
// Created by 25031 on 25-5-20.
//

#pragma once

#include <vector>
#include <memory>
#include <string>

namespace tool {
    class DbHelperImpl;
    class DbHelper {
    public:
        struct Data {
            int32_t id;
            int32_t priority;
            uint64_t timestamp;
            std::vector<char> content;
        };

    public:
        DbHelper(const std::string& db_path);

        ~DbHelper();

        void InsertData(const Data& data);

        void DeleteData(const Data& data);

        void DeleteDatas(const std::vector<Data>& datas);

        std::vector<Data> QueryData(int32_t limit);

    private:
        std::unique_ptr<DbHelperImpl> impl_;
    };
}



