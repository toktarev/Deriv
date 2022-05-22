#ifndef JSON_VALIDATOR_H
#define JSON_VALIDATOR_H

#include "rapidjson/schema.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

namespace Deriv {
    class JsonValidator {
    public:
        static std::string &authSchema() {
            static std::string SCHEMA = R"({
  "$schema": "http://json-schema.org/draft-04/schema#",
  "type": "object",
  "properties": {
    "authorize": {
      "type": "object",
      "properties": {
        "account_list": {
          "type": "array",
          "items": [
            {
              "type": "object",
              "properties": {
                "account_type": {
                  "type": "string"
                },
                "currency": {
                  "type": "string"
                },
                "is_disabled": {
                  "type": "integer"
                },
                "is_virtual": {
                  "type": "integer"
                },
                "landing_company_name": {
                  "type": "string"
                },
                "loginid": {
                  "type": "string"
                },
                "trading": {
                  "type": "object"
                }
              }
            }
          ]
        },
        "balance": {
          "type": "number"
        },
        "country": {
          "type": "string"
        },
        "currency": {
          "type": "string"
        },
        "email": {
          "type": "string"
        },
        "fullname": {
          "type": "string"
        },
        "is_virtual": {
          "type": "integer"
        },
        "landing_company_fullname": {
          "type": "string"
        },
        "landing_company_name": {
          "type": "string"
        },
        "local_currencies": {
          "type": "object",
          "properties": {
            "RUB": {
              "type": "object",
              "properties": {
                "fractional_digits": {
                  "type": "integer"
                }
              },
              "required": [
                "fractional_digits"
              ]
            }
          }
        },
        "loginid": {
          "type": "string"
        },
        "preferred_language": {
          "type": "string"
        },
        "scopes": {
          "type": "array",
          "items": [
            {
              "type": "string"
            },
            {
              "type": "string"
            },
            {
              "type": "string"
            },
            {
              "type": "string"
            },
            {
              "type": "string"
            }
          ]
        },
        "trading": {
          "type": "object"
        },
        "upgradeable_landing_companies": {
          "type": "array",
          "items": [
            {
              "type": "string"
            }
          ]
        },
        "user_id": {
          "type": "integer"
        }
      }
    },
    "echo_req": {
      "type": "object",
      "properties": {
        "authorize": {
          "type": "string"
        }
      }
    },
    "msg_type": {
      "type": "string"
    }
  }
})";
            return SCHEMA;
        }

        static std::string &buyContractSchema() {
            static std::string SCHEMA = R"({
  "$schema": "http://json-schema.org/draft-04/schema#",
  "type": "object",
  "properties": {
    "buy": {
      "type": "object",
      "properties": {
        "balance_after": {
          "type": "number"
        },
        "buy_price": {
          "type": "integer"
        },
        "contract_id": {
          "type": "integer"
        },
        "longcode": {
          "type": "string"
        },
        "payout": {
          "type": "number"
        },
        "purchase_time": {
          "type": "integer"
        },
        "shortcode": {
          "type": "string"
        },
        "start_time": {
          "type": "integer"
        },
        "transaction_id": {
          "type": "integer"
        }
      }
    },
    "echo_req": {
      "type": "object",
      "properties": {
        "buy": {
          "type": "integer"
        },
        "parameters": {
          "type": "object",
          "properties": {
            "amount": {
              "type": "integer"
            },
            "app_markup_percentage": {
              "type": "string"
            },
            "basis": {
              "type": "string"
            },
            "contract_type": {
              "type": "string"
            },
            "currency": {
              "type": "string"
            },
            "duration": {
              "type": "integer"
            },
            "duration_unit": {
              "type": "string"
            },
            "symbol": {
              "type": "string"
            }
          }
        },
        "price": {
          "type": "integer"
        },
        "subscribe": {
          "type": "integer"
        }
      }
    },
    "msg_type": {
      "type": "string"
    },
    "subscription": {
      "type": "object",
      "properties": {
        "id": {
          "type": "string"
        }
      }
    }
  }
}
)";
            return SCHEMA;
        }

        static std::string &ticksSchema() {
            static std::string SCHEMA = R"({
  "$schema": "http://json-schema.org/draft-04/schema#",
  "type": "object",
  "properties": {
    "echo_req": {
      "type": "object",
      "properties": {
        "contract_id": {
          "type": "string"
        },
        "proposal_open_contract": {
          "type": "integer"
        },
        "subscribe": {
          "type": "integer"
        }
      }
    },
    "msg_type": {
      "type": "string"
    },
    "proposal_open_contract": {
      "type": "object",
      "properties": {
        "account_id": {
          "type": "integer"
        },
        "barrier_count": {
          "type": "integer"
        },
        "bid_price": {
          "type": "number"
        },
        "buy_price": {
          "type": "integer"
        },
        "contract_id": {
          "type": "integer"
        },
        "contract_type": {
          "type": "string"
        },
        "currency": {
          "type": "string"
        },
        "current_spot": {
          "type": "number"
        },
        "current_spot_display_value": {
          "type": "string"
        },
        "current_spot_time": {
          "type": "integer"
        },
        "date_expiry": {
          "type": "integer"
        },
        "date_settlement": {
          "type": "integer"
        },
        "date_start": {
          "type": "integer"
        },
        "display_name": {
          "type": "string"
        },
        "expiry_time": {
          "type": "integer"
        },
        "id": {
          "type": "string"
        },
        "is_expired": {
          "type": "integer"
        },
        "is_forward_starting": {
          "type": "integer"
        },
        "is_intraday": {
          "type": "integer"
        },
        "is_path_dependent": {
          "type": "integer"
        },
        "is_settleable": {
          "type": "integer"
        },
        "is_sold": {
          "type": "integer"
        },
        "is_valid_to_cancel": {
          "type": "integer"
        },
        "is_valid_to_sell": {
          "type": "integer"
        },
        "longcode": {
          "type": "string"
        },
        "payout": {
          "type": "number"
        },
        "profit": {
          "type": "number"
        },
        "profit_percentage": {
          "type": "number"
        },
        "purchase_time": {
          "type": "integer"
        },
        "shortcode": {
          "type": "string"
        },
        "status": {
          "type": "string"
        },
        "transaction_ids": {
          "type": "object",
          "properties": {
            "buy": {
              "type": "integer"
            }
          },
          "required": [
            "buy"
          ]
        },
        "underlying": {
          "type": "string"
        },
        "validation_error": {
          "type": "string"
        }
      }
    },
    "subscription": {
      "type": "object",
      "properties": {
        "id": {
          "type": "string"
        }
      }
    }
  }
}
)";
            return SCHEMA;
        }

        static bool validateJson(const std::string &inputJson, const std::string &schemaJson, std::string &error) {
            rapidjson::Document sd{};

            if (sd.Parse(schemaJson.c_str()).HasParseError()) {
                return false;
            }

            rapidjson::SchemaDocument schema(sd);

            rapidjson::Document d{};

            if (d.Parse(inputJson.c_str()).HasParseError()) {
                return false;
            }

            rapidjson::SchemaValidator validator(schema);

            if (!d.Accept(validator)) {
                rapidjson::StringBuffer sb{};
                validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
                error = sb.GetString();
                return false;
            }

            return true;
        }
    };
}

#endif //JSON_VALIDATOR_H
