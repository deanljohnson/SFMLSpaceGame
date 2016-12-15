#pragma once
#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>

template<typename TKey, typename TData>
class GraphMap
{
public:
	class NeighborIterator;
private:
	struct Vertex
	{
		explicit Vertex(const TData& data) : data(data) {}
		TData data;
		std::vector<Vertex*> neighbors;

		NeighborIterator begin() { return NeighborIterator(neighbors.begin(), *this); }
		NeighborIterator end() { return NeighborIterator(neighbors.end(), *this); }
	};

	std::unordered_map<TKey, std::unique_ptr<Vertex>> m_vertices;
public:
	class NeighborIterator
	{
	private:
		typedef typename std::vector<Vertex*>::iterator base_iterator;
		Vertex* m_vertex;
		base_iterator m_iterator;
	public:
		NeighborIterator(base_iterator pos, Vertex& vert)
			: m_vertex(&vert), m_iterator(pos) {}
		NeighborIterator(const NeighborIterator& other)
			: m_vertex(other.m_vertex), m_iterator(other.m_iterator) {}

		NeighborIterator& operator=(const NeighborIterator& other)
		{
			m_vertex = other.m_vertex;
			m_iterator = other.m_iterator;
			return *this;
		}

		NeighborIterator& operator++()
		{
			++m_iterator;
			return *this;
		}

		NeighborIterator operator++(int)
		{
			NeighborIterator oldIt{*this};
			++m_iterator;
			return oldIt;
		}

		bool operator==(const NeighborIterator& other)
		{
			return m_vertex == other.m_vertex 
				&& m_iterator == other.m_iterator;
		}

		bool operator!=(const NeighborIterator& other)
		{
			return !(*this == other);
		}

		TData* operator->()
		{
			return &(*m_iterator)->data;
		}

		TData& operator*()
		{
			return (*m_iterator)->data;
		}
	};

	void Add(const TKey& key, const TData& data)
	{
		m_vertices.emplace(std::make_pair(key, std::make_unique<Vertex>(data)));
	}

	void Remove(const TKey& key)
	{
		auto& vertToRemove = m_vertices.at(key);

		// for each of the neighbors of the vertex being removed
		for (auto neighbor = vertToRemove->neighbors.begin(); 
					neighbor != vertToRemove->neighbors.end(); 
					++neighbor)
		{
			// on those neighbors, find the vertex being removed
			// and erase it
			for (auto it = (*neighbor)->neighbors.begin();
						it != (*neighbor)->neighbors.end();
						++it)
			{
				if (*it == &*vertToRemove)
				{
					(*neighbor)->neighbors.erase(it);
					break;
				}
			}
		}

		m_vertices.erase(key);
	}

	void Connect(const TKey& a, const TKey& b)
	{
		auto& aVert = m_vertices.at(a);
		auto& bVert = m_vertices.at(b);

		aVert->neighbors.push_back(&*bVert);
		bVert->neighbors.push_back(&*aVert);
	}

	void Disconnect(const TKey& a, const TKey& b)
	{
		auto& aVert = m_vertices.at(a);
		auto& bVert = m_vertices.at(b);

		// remove bVert from a's neighbors
		for (auto it = aVert->neighbors.begin(); 
			it != aVert->neighbors.end(); 
			++it)
		{
			if (*it == &*bVert)
			{
				aVert->neighbors.erase(it);
				break;
			}
		}

		// remove aVert from b's neighbors
		for (auto it = bVert->neighbors.begin(); 
			it != bVert->neighbors.end(); 
			++it)
		{
			if (*it == &*aVert)
			{
				bVert->neighbors.erase(it);
				break;
			}
		}
	}

	bool AreNeighbors(const TKey& a, const TKey& b)
	{
		auto& aVert = m_vertices.at(a);
		auto& bVert = m_vertices.at(b);

		// searching the smallest neighbors vector
		if (aVert->neighbors.size() <= bVert->neighbors.size())
		{
			for (auto& n : aVert->neighbors)
			{
				if (n == &*bVert)
					return true;
			}
		}
		else
		{
			for (auto& n : bVert->neighbors)
			{
				if (n == &*aVert)
					return true;
			}
		}

		return false;
	}

	// Performs BFS starting from the given key's vertex.
	// Applies the function to every TData in the search.
	// If the function returns false, the search is 
	// terminated, else it continues until all elements
	// have been visited
	void BreadthFirstTraverse(const TKey& start, std::function<bool(TData&)> func)
	{
		std::unordered_map<Vertex*, bool> visited;
		std::queue<Vertex*> pending;

		pending.push(m_vertices.at(start).get());
		while (!pending.empty())
		{
			Vertex* cur = pending.front();
			pending.pop();

			if(!func(cur->data))
				return;

			visited[cur] = true;

			for (auto& n : cur->neighbors)
			{
				if (!visited[n])
					pending.push(n);
			}
		}
	}

	// Performs DFS starting from the given key's vertex.
	// Applies the function to every TData in the search.
	// If the function returns false, the search is 
	// terminated, else it continues until all elements
	// have been visited
	void DepthFirstTraverse(const TKey& start, std::function<bool(TData&)> func)
	{
		std::unordered_map<Vertex*, bool> visited;
		std::stack<Vertex*> pending;

		pending.push(m_vertices.at(start).get());
		while (!pending.empty())
		{
			Vertex* cur = pending.top();
			pending.pop();

			if (!func(cur->data))
				return;

			visited[cur] = true;

			for (auto& n : cur->neighbors)
			{
				if (!visited[n])
					pending.push(n);
			}
		}
	}

	NeighborIterator NeighborBegin(const TKey& key)
	{
		return m_vertices.at(key)->begin();
	}

	NeighborIterator NeighborEnd(const TKey& key)
	{
		return m_vertices.at(key)->end();
	}

	bool Contains(const TKey& key) 
	{
		return m_vertices.find(key) != m_vertices.end();
	}

	TData& operator[](const TKey& key)
	{
		return m_vertices.at(key)->data;
	}
};
