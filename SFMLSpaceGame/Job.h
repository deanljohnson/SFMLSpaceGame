#pragma once

class Job
{
public:
	virtual ~Job()
	{
	}

	// Execute this job
	virtual void Do() = 0;
	// Returns whether or not this job is finished
	virtual bool IsFinished() = 0;
};