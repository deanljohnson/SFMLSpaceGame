// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <FindBestSaleJob.h>
#include <Economy.h>

FindBestSaleJob::FindBestSaleJob(ItemType typeToSell, const std::string& detail, 
	size_t amountToSell, const std::string& start, 
	size_t searchRange, 
	std::function<bool(const EconomyAgent&, Price&)> filter)
	: m_typeToSell(typeToSell),
	m_detail(detail),
	m_amountToSell(amountToSell),
	m_start(start),
	m_searchRange(searchRange),
	m_filter(filter)
{
}

void FindBestSaleJob::Do()
{
	std::lock_guard<std::mutex> lock(m_resultLock);
	m_result = Economy::FindBestSale(m_typeToSell, m_detail, m_amountToSell, m_start, m_searchRange, m_filter);
	m_finished.store(true);
}

bool FindBestSaleJob::IsFinished()
{
	return m_finished.load();
}

const FindSaleJobResult& FindBestSaleJob::GetResult()
{
	std::lock_guard<std::mutex> lock(m_resultLock);
	return m_result;
}