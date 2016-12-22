// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <FindBestPurchaseJob.h>
#include <Economy.h>

FindBestPurchaseJob::FindBestPurchaseJob(const std::string& start, size_t searchRange, std::function<bool(const EconomyAgent&, Price&, ItemType)> filter)
	: m_finished(false),
	m_result(),
	m_start(start),
	m_searchRange(searchRange),
	m_filter(filter)
{
}

void FindBestPurchaseJob::Do()
{
	std::lock_guard<std::mutex> lock(m_resultLock);
	m_result = Economy::FindBestPurchase(m_start, m_searchRange, m_filter);
	m_finished.store(true);
}

bool FindBestPurchaseJob::IsFinished()
{
	return m_finished.load();
}

const FindPurchaseJobResult& FindBestPurchaseJob::GetResult()
{
	std::lock_guard<std::mutex> lock(m_resultLock);
	return m_result;
}
