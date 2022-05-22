#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <array>
#include <mutex>

namespace Deriv {

    static const int SECONDS_IN_MINUTE = 60;

    static const int TICKS_TIMEOUT = SECONDS_IN_MINUTE * 1000;

    class Holder final {
    public:
        std::string payload;
        bool isSuccess = false;
        u_int64_t timeStamp = 0;

        Holder() = default;

        Holder(Holder &other) {
            this->payload = other.payload;
            this->isSuccess = other.isSuccess;
            this->timeStamp = other.timeStamp;
        }

        Holder(Holder &&other) noexcept {
            payload = std::move(other.payload);
            isSuccess = other.isSuccess;
            timeStamp = other.timeStamp;
        }

        Holder &operator=(Holder &&other) noexcept {
            this->payload = std::move(other.payload);
            this->timeStamp = other.timeStamp;
            this->isSuccess = other.isSuccess;
            return *this;
        }

        Holder &operator=(const Holder &other) noexcept = default;

        ~Holder() = default;
    };


    class CircularBuffer {
    public:
        explicit CircularBuffer() = default;

        void put(Holder &item) noexcept {
            std::unique_lock<std::mutex> lock(mutex_);

            if (full_) {
                cond.wait(lock);
                tail_ = (tail_ + 1) % SECONDS_IN_MINUTE;
            }

            buf_[head_] = item;
            head_ = (head_ + 1) % SECONDS_IN_MINUTE;

            full_ = head_ == tail_;
        }

        Holder peek() const noexcept {
            std::unique_lock<std::mutex> lock(mutex_);

            if (empty()) {
                Holder holder{};
                holder.isSuccess = false;
                return holder;
            }

            // Read data and advance the tail (we now have a free space)
            auto duration = std::chrono::system_clock::now().time_since_epoch();
            u_int64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

            if (now - buf_[tail_].timeStamp < TICKS_TIMEOUT) {
                Holder holder{};
                holder.isSuccess = false;
                return holder;
            }

            Holder holder = buf_[tail_];
            holder.isSuccess = true;
            full_ = false;
            tail_ = (tail_ + 1) % SECONDS_IN_MINUTE;
            cond.notify_all();
            return holder;
        }

    private:
        mutable std::mutex mutex_;
        mutable std::condition_variable cond;
        mutable std::array<Holder, SECONDS_IN_MINUTE> buf_;
        mutable size_t head_ = 0;
        mutable size_t tail_ = 0;
        mutable bool full_ = false;

        bool empty() const {
            return !full_ && (head_ == tail_);
        }
    };
}
#endif //CIRCULAR_BUFFER_H
