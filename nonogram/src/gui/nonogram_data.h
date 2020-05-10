#pragma once

#include <vector>
#include <memory>

#include "constans.h"

namespace Game{
    //Данные для нонограммы
    using HeaderData = std::vector<std::vector<int>>;
    using ResultData = std::vector<std::vector<State>>;

    struct NonogramData
    {
        HeaderData &getRows(){
            return rows;
        }

        HeaderData &getColumns(){
            return cols;
        }

        const ResultData &getResult() const {
            return result;
        }

        void setResult(ResultData &&res) {
            result = res;
        }

        void clear(){
            result.clear();
            rows.clear();
            cols.clear();
        }
    private:
        HeaderData rows;
        HeaderData cols;
        ResultData result;
    };

    using NonogramDataPtr = std::shared_ptr<NonogramData>;
}
