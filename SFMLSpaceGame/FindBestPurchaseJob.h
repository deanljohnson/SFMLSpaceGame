#pragma once
#include <functional>
#include <atomic>
#include <mutex>
#include <Job.h>
#include <ItemPrice.h>
#include <FindPurchaseJobResult.h>

class EconomyAgent;
enum class ItemType;

class FindBestPurchaseJob : public Job
{
private:
	std::mutex m_resultLock;

	std::atomic<bool> m_finished;
	FindPurchaseJobResult m_result;

	std::string m_start;
	size_t m_searchRange;
	std::function<bool(const EconomyAgent&, Price&, ItemType)> m_filter;
public:
	explicit FindBestPurchaseJob(const std::string& start = "",
		size_t searchRange = 0,
		std::function<bool(const EconomyAgent&, Price&, ItemType)> filter = nullptr);

	virtual void Do() override;
	virtual bool IsFinished() override;
	const FindPurchaseJobResult& GetResult();
};