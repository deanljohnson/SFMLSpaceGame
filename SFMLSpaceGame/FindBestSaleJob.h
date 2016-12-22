#pragma once
#include <functional>
#include <atomic>
#include <mutex>
#include <Job.h>
#include <ItemPrice.h>
#include <FindSaleJobResult.h>

class EconomyAgent;
enum class ItemType;

class FindBestSaleJob : public Job
{
private:
	std::mutex m_resultLock;

	std::atomic<bool> m_finished;
	FindSaleJobResult m_result;

	ItemType m_typeToSell;
	std::string m_detail;
	size_t m_amountToSell;
	std::string  m_start;
	size_t m_searchRange;
	std::function<bool(const EconomyAgent&, Price&)> m_filter;
public:
	FindBestSaleJob(ItemType typeToSell, const std::string& detail, size_t amountToSell,
		const std::string& start = "",
		size_t searchRange = 0,
		std::function<bool(const EconomyAgent&, Price&)> filter = nullptr);

	virtual void Do() override;
	virtual bool IsFinished() override;
	const FindSaleJobResult& GetResult();
};