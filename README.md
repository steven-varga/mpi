MPI C++ proposal
---------------- 

Strong similarity between MPI and HDF5 systems allows significant code/pattern reuse from H5CPP project. While there are naming differences in concepts such as HDF5 property lists vs MPI_info, the main building blocks remain the same:

- type system
- static reflection
- handles/identifier: RAII, 
- daisy chaining properties can easily be converted into setting MPI_info key/value pairs where needed
- metaprogramming based pythonic intuitive syntax
- feature detection idiom based mechanism to support STL like containers: `std::vector` ... 




### Features
- static reflection based on LLVM/clang
- PBLAS/SCALAPACK support: block cycling distribution
- full interop with C MPI code though resource/handle automatic conversion
- full interop with HDF5 systems with H5CPP by the same author
- full linear algebra support for major linear algebra systems
- full STL like object support with feature detection idiom based approach
- generative programming: only the necessary code is instantiated
- RAII idiom to make sure resources are closed
- H5CPP style structured exceptions: `mpi::error::any` rules them all
- intuitive syntax

# Examples

### handles arbitrary complex pod_t type, based on `h5cpp-compiler` and LLVM based static reflection tool:

```cpp
#include <h5mpi>
...
int main(int argc, char* argv[]){
    mpi::init(); // optional, will register mpi::finalize with std::atexit
    int total = elements_per_proc * world_size;
    { // scatter/gather with local buffer predefined
        std::vector<int> local(elements_per_proc);
        mpi::scatter(
            std::vector<int>(total, 42), local, mpi::world);
    }
    { // scatter/gather: arguments may be passed in arbitrary order, `mpi::world` may be implicit
      // `segment` uses RVO, with size == elements_per_proc, filled with the correct values for
      //  each rank 
        auto segment =  mpi::scatter(mpi::world, std::vector<struct_t>(total, struc_t{..}));
        float sub_avg = std::accumulate(segment.begin(), segment.end(), std::multiplies<int>());
        auto result = mpi::gather(sub_avg);
        if(rank != 0) // on non participating ranks will cost a few bytes, rank_0 has all data
            assert(result.size() == 0);
    }
} 
```

Works seamlessly with HDF5 datasystem:
```cpp
#include <h5mpi>
#include <h5cpp>
...
int main(int argc, char* argv[]){
  mpi::init(argc, argv);
  mpi::comm_t comm; // defaults to MPI_WORLD
  mpi::info info;   // similar to property lists 
  size_t rank = mpi::rank(comm), size = mpi::size(comm);
  auto fd = h5::open("container.h5", h5::fcpl, h5::mpiio({comm, info}) );
   
  arma::mat M(height, width, ... );
  h5::read(fd, "dataset", M, h5::collective | h5::block_cyclic | h5::independent,
    h5::offset{..}, h5::count{..});    
  mpi::finalize()
}
```
another example, reworked from 'ring.cxx'
```cpp

#include "h5mpi/all"
#include <iostream>

namespace some {
    struct pod_t{
        size_t count,
        int array[10],
        ... arbitrary complex, cascading .. 
    };
}


int main(int argc, char *argv[]) {
    mpi::init();
    mpi::comm_t comm; // defaults to MPI_WORLD, this could be implicit
    int = mpi::rank(comm), size = mpi::size(comm);
    int next = (rank + 1) % size, 
    prev = (rank + size - 1) % size, tag;

    if (rank == 0) mpi::send( 
        // passed as reference, object created in place
        some::pod_t{..}, next [, tag]);

    while (true) { // passing the message around
        // RVO, needs template specialization
        some::pod_t message = mpi::receive<some::pod_t>(prev, tag);
        if (rank == 0) -- message.count; 

        mpi::send(message, next, tag);
        if (message.count == 0) // exiting
            break;
    }
    some::pod_t message; 
    if (rank == 0) // message passed by reference, will be updated
        mpi::receive(message, prev, tag);
    // with pod_t types pointer passing also works out of the box, and my be more idiomatic
    // mpi::receive(&message, prev, tag); 
    // however this pattern is overly restrictive when applied to STL like containers 
    MPI::Finalize();
    return 0;
}

```

### context
- `mpi::init`
- `mpi::thread`

### list of resources, similar to HDF5 `hid_t` descriptors
- `mpi::comm`      `mpi::comm_t`
- `mpi::window`    `mpi::win_t`
- `mpi::type`      `mpi::tp_t`
- `mpi::request`   `mpi::req_t`
- `mpi::attribute` `mpi::attr_t`
- `mpi::cartesian` `mpi::cart_t`
- `mpi::group`     `mpi::grp_t`
- `mpi::file`      `mpi::fd_t`
- `mpi::info`      `mpi::inf_t`


#### create
- `mpi::comm_t mpi::create(const mpi::grp_t& group)` same as `ctor`-s
- `mpi::comm_t mpi::create(const mpi::comm_t& comm, const mpi::grp_t& group)`
- `mpi::comm_t mpi::group(const mpi::comm_t& comm, const mpi::grp_t& group)`
- `mpi::comm_t mpi::split(const mpi::comm_t& comm, int color, int key)`
- `mpi::comm_t mpi::dup(const mpi::comm_t& comm)` same as copy ctor
- `mpi::comm_t mpi::connect(const mpi::comm_t& comm, const std::string& name, int root, const mpi::inf_t& info)`

#### window
`MPI_info` is similar mechanism to HDF5 property lists to store side band information, mostly to fine tune behaviour. 
`mpi::shared | mpi::no_locks | mpi::accumulate_ordering | mpi::accumulate_ops | mpi::same_size | same_disp_unit`


- `mpi::win_t mpi::allocate(const mpi::comm_t& comm, size_t size, size_t disp_unit [, error_handler] [,properties])`


### list of operators
- `mpi::req_t mpi::request<T>(mpi::comm& comm, int rank, int tag)`
- `mpi::create` | `mpi::delete`
- `mpi::dup`
- `mpi::abort( mpi::req_t&, int error_code )`
- `mpi::accumulate( origin, target, op)`
- `mpi::split()`
- `mpi::test`
- `mpi::malloc`
- `mpi::barrier`
- `mpi::broadcast`
- `mpi::send`
- `mpi::receive`
- `mpi::cancel`
- `mpi::accept`
- `mpi::group` -- this should be replaced with 'mpi::???'
- `mpi::join`
- `mpi::rank`
- `mpi::size`
- `mpi::spawn`
- `mpi::split`
- `mpi::dimension`
- `mpi::graph`
- `mpi::adjacent`
- `mpi::neighbours`
- `mpi::scan`
- `mpi::free`
- `mpi::get`
- `mpi::address`
- `mpi::count`
- `mpi::elements`
- `mpi::map`
- `mpi::intersect`
- `mpi::difference`
- `mpi::probe`
- `mpi::receive`
- `mpi::lookup`
- `mpi::publish`

- `mpi::is_finalized`
- `mpi::is_initilized`
- `mpi::is_thread`
- `mpi::gather`
- `mpi::scatter`
- `mpi::flush`
- `mpi::attach`
- `mpi::detach`
- `mpi::lock`
- `mpi::unlock`
- `mpi::post`
- `mpi::tick`
- `mpi::time`

### property lists
Similarly to HDF5 systems MPI has a way to provide side band information in MPI_info object as 
key value pairs.

- `mpi::all`
- `mpi::all_to_all`
- `mpi::any`
- `mpi::some`
- `mpi::no_locks`
- `mpi::exclusive`
- `mpi::inclusive`
- `mpi::non-blocking`
- `mpi::blocking`
- `mpi::buffered`
- `mpi::local`
- `mpi::remote`
- `mpi::sync` | `mpi::async`

