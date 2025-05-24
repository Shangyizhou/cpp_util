//
// Created by 25031 on 25-5-20.
//

#include "DbHelper.h"
#include "sqlite_orm/sqlite_orm.h"

using namespace sqlite_orm;

namespace tool {

    inline auto InitStorage(const std::string& path) {
        return make_storage(
            path, make_table("buried_data",
                             make_column("id", &DbHelper::Data::id, primary_key().autoincrement()),
                             make_column("priority", &DbHelper::Data::priority),
                             make_column("timestamp", &DbHelper::Data::timestamp),
                             make_column("content", &DbHelper::Data::content)));
    }


    class DbHelperImpl {
    public:
        using DBStorage = decltype(InitStorage(""));

    public:
        DbHelperImpl(const std::string& dp_path) : db_path_(dp_path) {
            storage_ = std::make_unique<DBStorage>(InitStorage(db_path_));
            storage_->sync_schema();
        }

        ~DbHelperImpl() {}

        void InsertData(const DbHelper::Data& data) {
            auto guard = storage_->transaction_guard();
            storage_->insert(data);
            guard.commit();
        }

        void DeleteData(const DbHelper::Data& data) {
            auto guard = storage_->transaction_guard();
            storage_->remove_all<DbHelper::Data>(
                where(c(&DbHelper::Data::id) == data.id));
            guard.commit();
        }

        void DeleteDatas(const std::vector<DbHelper::Data>& datas) {
            auto guard = storage_->transaction_guard();
            for (const auto& data : datas) {
                storage_->remove_all<DbHelper::Data>(
                    where(c(&DbHelper::Data::id) == data.id));
            }
            guard.commit();
        }

        std::vector<DbHelper::Data> QueryData(int32_t limit_size) {
            auto limited = storage_->get_all<DbHelper::Data>(
                order_by(&DbHelper::Data::priority).desc(), limit(limit_size));
            return limited;
        }

    private:
        std::string db_path_;

        std::unique_ptr<DBStorage> storage_{};
    };

    DbHelper::DbHelper(const std::string& db_path)
        : impl_(std::make_unique<DbHelperImpl>(std::move(db_path))) {}

    DbHelper::~DbHelper() {}

    void DbHelper::InsertData(const Data& data) { impl_->InsertData(data); }

    void DbHelper::DeleteData(const Data& data) { impl_->DeleteData(data); }

    void DbHelper::DeleteDatas(const std::vector<Data>& datas) {
        impl_->DeleteDatas(datas);
    }

    std::vector<DbHelper::Data> DbHelper::QueryData(int32_t limit) {
        return impl_->QueryData(limit);
    }
} // namespace tracker
