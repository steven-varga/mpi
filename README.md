# H5MPI documentation 

### Features
- static reflection based on LLVM/clang
- PBLAS/SCALAPACK support: block cycling distribution
- full interop with C MPI code though resource/handle automatic conversion
- full interop with HDF5 systems with H5CPP by the same author
- full linear algebra support for major linear algebra systems
- full STL like object support with feature detection based approach
- generative programming: only the necessary code is intatiated
- RAII idiom to make sure resources are closed
- H5CPP style structured exceptions: `mpi::error::any` rules them all

### Syntax

- `mpi::comm comm` creates MPI_WORLD
- `mpi::comm comm{}`


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


### list of operators
- `mpi::create` | `mpi::delete`

- `mpi::abort( mpi::request )`
- `mpi::accumulate( origin, target, op)`
- `mpi::split`
- `mpi::test`
- `mpi::gather`
- `mpi::scatter`
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

- `mpi::flush`
- `mpi::attach`
- `mpi::detach`
- `mpi::lock`
- `mpi::unlock`
- `mpi::post`
- `mpi::tick`
- `mpi::time`

### property lists
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

