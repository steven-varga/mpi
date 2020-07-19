# H5MPI documentation 
Strong similarity between MPI and HDF5 systems allows significant code/pattern reuse from H5CPP project. While there are naming differences in concepts such as HDF5 property lists vs MPI_info, the main building blocks remain the same:

- type system
- static reflection
- handles/identifier: RAII, 
- daisy chaining properties can easily be converted into setting MPI_info key/value pairs where needed
- metaprogramming based pythonic easy syntax
- STL like feature detection based container approach




### Features
- static reflection based on LLVM/clang
- PBLAS/SCALAPACK support: block cycling distribution
- full interop with C MPI code though resource/handle automatic conversion
- full interop with HDF5 systems with H5CPP by the same author
- full linear algebra support for major linear algebra systems
- full STL like object support with feature detection based approach
- generative programming: only the necessary code is instantiated
- RAII idiom to make sure resources are closed
- H5CPP style structured exceptions: `mpi::error::any` rules them all
- intuitive syntax





### context
- `mpi::init`
- `mpi::thread`

### list of resources
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

