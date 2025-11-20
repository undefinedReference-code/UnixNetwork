#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include "unp.h"
#include <algorithm>
#include <numeric>

class DaytimeClient {
public:
    struct TestResult {
        size_t totalRequests = 0;
        size_t successRequests = 0;
        std::vector<double> responseTimes; // 单位：毫秒
        std::chrono::duration<double> testDuration;
    };

    TestResult runTest(const std::string& host, uint16_t port, int numClients, int requestsPerClient, double spawnRate) {
        TestResult result;
        auto startTime = std::chrono::high_resolution_clock::now();
        std::vector<std::thread> threads;
        std::mutex resultMutex;

        for (int i = 0; i < numClients; ++i) {
            threads.emplace_back([&, i]() {
                std::this_thread::sleep_for(std::chrono::duration<double>(i / spawnRate));
                
                auto clientResult = runSingleClient(host, port, requestsPerClient);
                
                std::lock_guard<std::mutex> lock(resultMutex);
                result.totalRequests += clientResult.totalRequests;
                result.successRequests += clientResult.successRequests;
                result.responseTimes.insert(result.responseTimes.end(), 
                                           clientResult.responseTimes.begin(), 
                                           clientResult.responseTimes.end());
            });
        }

        for (auto& t : threads) {
            if (t.joinable()) t.join();
        }

        result.testDuration = std::chrono::high_resolution_clock::now() - startTime;
        return result;
    }

private:
    double performDaytimeRequest(const std::string& host, uint16_t port) {
        int sockfd = -1;
        auto start = std::chrono::high_resolution_clock::now();

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            return -1;
        }

        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);
        if (inet_pton(AF_INET, host.c_str(), &servaddr.sin_addr) <= 0) {
            close(sockfd);
            return -1;
        }

        if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) < 0) {
            close(sockfd);
            return -1;
        }

        char recvline[MAXLINE + 1];
        ssize_t n;
        while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
            // successfully get data
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        close(sockfd);

        if (n < 0) { // error
            return -1;
        }
        return duration.count() / 1000.0; // ms
    }

    // 然后 runSingleClient 函数可以简化为：
    TestResult runSingleClient(const std::string& host, uint16_t port, int numRequests) {
        TestResult clientResult;
        for (int i = 0; i < numRequests; ++i) {
            double responseTime = performDaytimeRequest(host, port);
            clientResult.totalRequests++;
            if (responseTime >= 0) {
                clientResult.successRequests++;
                clientResult.responseTimes.push_back(responseTime);
            } else {
                // error
                clientResult.responseTimes.push_back(0); 
            }
            if (i < numRequests - 1) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
        return clientResult;
    }

};

void printReport(const DaytimeClient::TestResult& result) {
    if (result.responseTimes.empty()) {
        std::cout << "failed to get data" << std::endl;
        return;
    }
    
    std::vector<double> sortedTimes = result.responseTimes;
    std::sort(sortedTimes.begin(), sortedTimes.end());
    
    // Time
    double totalTime = std::accumulate(sortedTimes.begin(), sortedTimes.end(), 0.0);
    double averageTime = totalTime / sortedTimes.size();
    double minTime = sortedTimes.front();
    double maxTime = sortedTimes.back();
    
    auto p50Index = static_cast<size_t>(sortedTimes.size() * 0.5);
    auto p90Index = static_cast<size_t>(sortedTimes.size() * 0.9);
    auto p95Index = static_cast<size_t>(sortedTimes.size() * 0.95);
    auto p99Index = static_cast<size_t>(sortedTimes.size() * 0.99);
    
    double p50 = p50Index < sortedTimes.size() ? sortedTimes[p50Index] : 0;
    double p90 = p90Index < sortedTimes.size() ? sortedTimes[p90Index] : 0;
    double p95 = p95Index < sortedTimes.size() ? sortedTimes[p95Index] : 0;
    double p99 = p99Index < sortedTimes.size() ? sortedTimes[p99Index] : 0;
    
    // QPS
    double totalSeconds = result.testDuration.count();
    double qps = totalSeconds > 0 ? result.successRequests / totalSeconds : 0;
    
    double successRate = result.totalRequests > 0 ? 
        (static_cast<double>(result.successRequests) / result.totalRequests * 100) : 0;
    
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "Daytime test report" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << "Total request: " << result.totalRequests << std::endl;
    std::cout << "Success request: " << result.successRequests << std::endl;
    std::cout << "Success Rate: " << successRate << "%" << std::endl;
    std::cout << "Total seconds: " << totalSeconds << " s" << std::endl;
    std::cout << "(QPS): " << qps << std::endl;
    
    std::cout << "\n--- lagency (ms) ---" << std::endl;
    std::cout << "avg: " << averageTime << " ms" << std::endl;
    std::cout << "min: " << minTime << " ms" << std::endl;
    std::cout << "max: " << maxTime << " ms" << std::endl;
    std::cout << "P50: " << p50 << " ms" << std::endl;
    std::cout << "P90: " << p90 << " ms" << std::endl;
    std::cout << "P95: " << p95 << " ms" << std::endl;
    std::cout << "P99: " << p99 << " ms" << std::endl;
    
    std::cout << "\n--- distribution ---" << std::endl;
    std::vector<std::pair<double, std::string>> ranges = {
        {1, "0-1ms"}, {5, "1-5ms"}, {10, "5-10ms"}, {20, "10-20ms"}, 
        {50, "20-50ms"}, {100, "50-100ms"}, {1000, "100-1000ms"}
    };
    
    for (size_t i = 0; i < ranges.size(); ++i) {
        size_t count = 0;
        for (double time : sortedTimes) {
            if (i == ranges.size() - 1) {
                if (time >= ranges[i].first) count++;
            } else {
                if (time >= ranges[i].first && time < ranges[i+1].first) count++;
            }
        }
        double percentage = static_cast<double>(count) / sortedTimes.size() * 100;
        std::cout << ranges[i].second << ": " << count << " request (" 
                  << percentage << "%)" << std::endl;
    }
    
    std::cout << std::string(60, '=') << std::endl;
}


int main() {
    DaytimeClient client;
    
    // Test: port 13，10 client，5 request each 2 clients pre sec
    auto result = client.runTest("127.0.0.1", 13, 10000, 5, 200.0);
    
    // Report
    printReport(result);
    
    return 0;
}