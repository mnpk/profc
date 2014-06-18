#pragma once
#include <boost/thread/thread.hpp>
#include <boost/atomic.hpp>
#include <boost/timer/timer.hpp>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>

#define CONCAT_DIRECT(x, y) x##y
#define CONCAT(x, y) CONCAT_DIRECT(x, y)
#define GETUNIQUENAME(x) CONCAT(x, __LINE__)
#define DEFINE_PROFILE(node_path) \
	static StaticProfileNodePtr GETUNIQUENAME(__node_ptr)(node_path);\
	ScopedProfile GETUNIQUENAME(__scoped_profile)(GETUNIQUENAME(__node_ptr).node())
#define theProfileManager ProfileManager::Instance()
#define RUNTIME_PROFILING "RUNTIME_PROFILING"

class ProfileNode
{
public:
	explicit ProfileNode(const std::string& name) : _name(name), _count(0), _elapsed_total(0) {}
	~ProfileNode() {}
	static bool greater(const ProfileNode* lhs, const ProfileNode* rhs)
	{
		return (lhs->elapsed_total() > rhs->elapsed_total());
	}
	void AccumulateTime(boost::timer::nanosecond_type time)
	{
		_count++;
		_elapsed_total += time;
	}
	// getter
	boost::timer::nanosecond_type elapsed_total() const {return _elapsed_total;}
	const std::string& name() const {return _name;}
	int count() const {return _count;}
private:
	std::string _name;
	boost::atomic<int> _count;
	boost::atomic<boost::timer::nanosecond_type> _elapsed_total;
};

class ProfileManager
{
public:
	ProfileManager() :_done(false)
	{
		_threads.create_thread(boost::bind(&ProfileManager::Run, this));
	}
	~ProfileManager()
	{
		while(!_nodes.empty()) delete _nodes.back(), _nodes.pop_back();
	}
	static ProfileManager& Instance()
	{
		static ProfileManager manager;
		return manager;
	}
	void Stop()
	{
		_done = true;
		_threads.join_all();
	}
	ProfileNode* AddNode(const std::string& name)
	{
		ProfileNode* node = new ProfileNode(name);
		boost::mutex::scoped_lock lock(_lock);
		_nodes.push_back(node);
		return node;
	}
	void PrintAllNodes()
	{
		using namespace std;
		cout << "-----------------------------------------------------------------------" << endl;
		cout << setw(25) << left << "name" << right << setw(11) << "rate" << setw(12) << "total" << setw(10) << "count" << setw(12) << "us/call" << endl;
		boost::mutex::scoped_lock lock(_lock);
		sort(_nodes.begin(), _nodes.end(), ProfileNode::greater);
		for (vector<ProfileNode*>::iterator itr = _nodes.begin(); itr != _nodes.end(); ++itr)
		{
			PrintNode(*itr);
		}
	}
	void AccumulateTime(ProfileNode* node, boost::timer::nanosecond_type time)
	{
		node->AccumulateTime(time);
		_elapsed_total += time;

	}
	boost::atomic<boost::timer::nanosecond_type> _elapsed_total;
private:
	void PrintNode(const ProfileNode* node)
	{
		using namespace std;
		cout << setw(25) << left << node->name() << right;
		cout << setw(10) << (node->elapsed_total() * 100 / _elapsed_total)  << "%";
		cout << setw(10) << node->elapsed_total() / 1000000 << "ms";
		cout << setw(10) << node->count();
		cout << setw(10) << (node->elapsed_total() / node->count() / 1000.0) << "us" << endl;
	}
	void Run()
	{
		while(!_done)
		{
			usleep(1000*1000);
			if (std::getenv(RUNTIME_PROFILING) && !_nodes.empty())
			{
				PrintAllNodes();
			}
		}
	}
private:
	std::vector<ProfileNode*> _nodes;
	boost::thread_group _threads;
	boost::atomic<bool> _done;
	boost::mutex _lock;
};

class StaticProfileNodePtr
{
public:
	StaticProfileNodePtr(const std::string& name)
	{
		_node = theProfileManager.AddNode(name);
	}
	~StaticProfileNodePtr()
	{
		theProfileManager.Stop();
	}
	ProfileNode* node() {return _node;}
private:
	ProfileNode* _node;
};

class ScopedProfile
{
public:
	explicit ScopedProfile(ProfileNode* node) :_node(node) {}
	~ScopedProfile()
	{
		theProfileManager.AccumulateTime(_node, _timer.elapsed().wall);
	}
private:
	ProfileNode* _node;
	boost::timer::cpu_timer _timer;
};

