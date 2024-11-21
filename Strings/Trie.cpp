#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct TrieNode
{
	bool isEndOfWord;
	int count = 0;
	TrieNode *children[26];
	TrieNode(bool isEndOfWord = false) : isEndOfWord{isEndOfWord}
	{

		for (int i = 0; i < 26; i++)
			children[i] = nullptr;
	}
};
class Trie
{

public:
	TrieNode *head;
	Trie()
	{
		this->head = new TrieNode();
	}

	void insert(string &word)
	{
		auto node = this->head;
		for (int i = 0; i < word.size(); i++)
		{
			int curr = word[i] - 'a';
			if (node->children[curr] == NULL)
			{
				node->children[curr] = new TrieNode();
			}
			node = node->children[curr];
			node->count++;
		}

		node->isEndOfWord = true;
	}

	bool search(string &word)
	{
		TrieNode *node = head;
		int curr = 0;

		for (int i = 0; i < word.size(); i++)
		{
			curr = word[i] - 'a';
			if (node->children[curr] == NULL)
			{
				return false;
			}
			node = node->children[curr];
		}

		return node->isEndOfWord;
	}

	bool startsWith(string &prefix)
	{
		TrieNode *node = head;
		int curr = 0;

		for (int i = 0; i < prefix.size(); i++)
		{
			curr = prefix[i] - 'a';
			if (node->children[curr] == NULL)
			{
				return false;
			}
			node = node->children[curr];
		}

		return true;
	}
	int countPrefix(string &prefix)
	{
		TrieNode *node = head;
		int curr = 0;

		for (int i = 0; i < prefix.size(); i++)
		{
			curr = prefix[i] - 'a';
			if (node->children[curr] == NULL)
			{
				return 0;
			}
			node = node->children[curr];
		}

		return node->count;
	}
	long long sum_all_prfs(string &prefix)
	{
		TrieNode *node = head;
		int curr = 0;
		long long ans = 0;
		for (int i = 0; i < prefix.size(); i++)
		{
			curr = prefix[i] - 'a';
			if (node->children[curr] == NULL)
			{
				return 0;
			}
			node = node->children[curr];
			ans += node->count;
		}

		return ans;
	}
};
