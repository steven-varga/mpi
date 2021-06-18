MPI C++ proposal
---------------- 

Strong similarity between MPI and HDF5 systems allows significant code/pattern reuse from H5CPP project. While there are naming differences in concepts such as HDF5 property lists vs MPI_info, the main building blocks remain the same:

- type system
- static reflection
- handles/identifier: RAII, 
- daisy chaining properties can easily be converted into setting MPI_info key/value pairs where needed
- intuitive pythonic syntax with template metaprogramming
- feature detection idiom based mechanism to support STL like containers: `std::vector` ... 




### Features
- static reflection based on LLVM/clang
- PBLAS/SCALAPACK support: block cycling distribution
- full interop with C MPI code: handles are binary equivalent
- full interop with HDF5 systems with H5CPP by the same author
- full linear algebra support for major linear algebra systems: armadillo, eigen3, ... 
- full STL like object support with feature detection idiom based approach
- generative programming: only the necessary code is instantiated
- RAII idiom to make sure resources are closed
- H5CPP style structured exceptions: `mpi::error::any` rules them all
- intuitive syntax

# Examples

The proposed mpi c++ will be able to handle arbitrary complex pod_t type -- type descriptors generated at compile time --; based on `h5cpp-compiler` and LLVM based static reflection tool, keeping the option open to switch over to purely 
template based reflection when it becomes available. In the following reworked from ['ring.cxx']() example.

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
    // registers mpi::finalize() at exit, similar behaviour of python and julia
    mpi::init(); 
    // it would be nice to have global variable `mpi::rank` as opposed to 
    // a function call; but  
    int next = (mpi::rank() + 1) % mpi::size(),
    prev = (mpi::rank() + mpi::size() - 1) % mpi::size() [, tag];

    if (mpi::rank() == 0) mpi::send( 
        // passed as reference, object created in place
        some::pod_t{..}, next [, tag]);

    while (true) { // passing the message around
        // RVO, needs template specialization
        some::pod_t message = mpi::receive<some::pod_t>(prev [, tag]);
        if (mpi::rank() == 0) -- message.count; 

        mpi::send(message, next [, tag]);
        if (message.count == 0) // exiting
            break;
    }
    some::pod_t message; 
    if (mpi::rank() == 0) // message passed by reference, will be updated
        mpi::receive(message, prev, tag);
    
    return 0; // mpit::finalize() gets called 
}
```

### another example with scatter - gather operation for containers
```cpp
#include <h5mpi>
...
int main(int argc, char* argv[]){
    mpi::init(); // will register mpi::finalize with std::atexit
    int total = elements_per_proc * world_size;
    // scatter/gather: arguments may be passed in arbitrary order, `mpi::world` may be implicit
    // `segment` uses RVO, with size == elements_per_proc, filled with the correct values for
    //  each rank 
    std::vector<float> segment = mpi::scatter(
        std::vector<float>(total, 1.0)   // data being spread out
        [,mpi::root{0}] [, mpi::world],  // optional values
        [,mpi::count{5}] );              // blocks/rank
    float partial_sum = std::reduce(segment.begin(), segment.end(), std::plus)
    std::vector<float> result = mpi::gather(partial_sum); // world_size
    if(rank != 0) // no participating ranks must return a valid container with zero elements
        assert(result.size() == 0);
} 
```
A possible code snipet, to demonstrate smooth interaction between linear algebra systems, HDF5 and MPI:
```cpp
#include <h5mpi>
#include <h5cpp>
...
int main(int argc, char* argv[]){
  mpi::init(argc, argv);  // registers mpi::finalize at exit
  mpi::comm_t comm;       // defaults to MPI_WORLD
  mpi::info info;         // similar to property lists 
  size_t rank = mpi::rank(comm), size = mpi::size(comm);
  auto fd = h5::open("container.h5", h5::fcpl, h5::mpiio({comm, info}) );
   
  arma::mat A(height, width, ... ), B(...), C(...);
  // reads a block accroding to block cycling distribution or entire dataset
  // into armadillo matrix, making it compatible with PBLAS, no scatter needed
  // instead implicit MPI file IO calls behind the scenes distribute data to each rank
  h5::read(fd, "dataset/A", A, h5::block_cyclic, h5::offset{..}, h5::count{..});
  h5::read(fd, "dataset/B", B, h5::block_cyclic, h5::offset{..}, h5::count{..});
  // parallel matrix matrix multiply, results in local C
  PvGEMM('N', 'T', A.nrows, A.n_cols, REAL, 
    A.memptr(), 0,0, desc_a,
    B.memptr(), 0,0, desc_b, REAL,
    C.memptr(), 0, 0, desc_c);
  // save result into an HDF5 container
  h5::write(fd, "result/C", C, h5::block_cyclic);
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

