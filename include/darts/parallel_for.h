/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2021 by Wojciech Jarosz
*/

#pragma once

#include <functional>
#include <future>
#include <vector>

/*!
  Executes the body of a for loop in parallel.

  \param begin		    The starting index of the for loop
  \param end 		    One past the ending index of the for loop
  \param step 		    How much to increment at each iteration when moving from begin to end
  \param body 		    The body of the for loop as a lambda, taking two parameters: the iterator index in [begin,end),
                        and the CPU number.
  \param num_threads 	Specify the number of threads to use. 0: as many as there are cores; 1:
                        execute in serial

  adapted from http://www.andythomason.com/2016/08/21/c-multithreading-an-effective-parallel-for-loop/
  license unknown, presumed public domain
 */
inline void parallel_for(int begin, int end, int step, std::function<void(int, size_t)> body, size_t num_threads = 0)
{
    std::atomic<int> next_index;
    next_index = begin;

    auto                           policy   = num_threads == 1 ? std::launch::deferred : std::launch::async;
    size_t                         num_CPUs = num_threads == 0 ? std::thread::hardware_concurrency() : num_threads;
    std::vector<std::future<void>> futures(num_CPUs);
    for (size_t cpu = 0; cpu != num_CPUs; ++cpu)
    {
        futures[cpu] = std::async(policy,
                                  [cpu, &next_index, end, step, &body]()
                                  {
                                      // just iterate, grabbing the next available atomic index in the range [begin,
                                      // end)
                                      while (true)
                                      {
                                          int i = next_index.fetch_add(step);
                                          if (i >= end)
                                              break;
                                          body(i, cpu);
                                      }
                                  });
    }
    for (auto &f : futures)
        f.get();
}

/// A version of the parallel_for accepting a body lambda that only takes the iterator index as a parameter
inline void parallel_for(int begin, int end, int step, std::function<void(int)> body, size_t num_threads = 0)
{
    parallel_for(
        begin, end, step, [&body](int i, size_t) { body(i); }, num_threads);
}

/// A version of parallel_for with a step of 1, providing the integer index and thread number to the loop \p body
inline void parallel_for(int begin, int end, std::function<void(int, size_t)> body, size_t num_threads = 0)
{
    parallel_for(begin, end, 1, body, num_threads);
}

/// A version of parallel_for with a step of 1, providing just the integer index to the loop \p body
inline void parallel_for(int begin, int end, std::function<void(int)> body, size_t num_threads = 0)
{
    parallel_for(begin, end, 1, body, num_threads);
}

/**
    \file
    \brief #parallel_for constructs
*/