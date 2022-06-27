// self-assign neglected
auto& operator=(this_class const& o)
  noexcept(noexcept(clear()) && noexcept(insert(o.begin(), o.end())))
  requires(std::is_copy_constructible_v<value_type>)
{
  clear();
  insert(o.begin(), o.end());

  return *this;
}

auto& operator=(this_class&& o)
  noexcept(noexcept(detail::destroy(root_, {})))
{
  detail::destroy(root_, {});

  root_ = o.root_;
  o.root_ = {};

  return *this;
}

auto& operator=(std::initializer_list<value_type> l)
  noexcept(noexcept(clear()) && noexcept(insert(l.begin(), l.end())))
  requires(std::is_copy_constructible_v<value_type>)
{
  clear();
  insert(l.begin(), l.end());

  return *this;
}

//
friend bool operator==(this_class const& lhs, this_class const& rhs) noexcept
{
  return std::equal(
    lhs.begin(), lhs.end(),
    rhs.begin(), rhs.end(),
    [](auto&& a, auto&& b) noexcept
    {
      return std::remove_cvref_t<decltype(lhs)>::node::cmp(a, b) == 0;
    }
  );
}

friend auto operator<=>(this_class const& lhs, this_class const& rhs) noexcept
{
  return std::lexicographical_compare_three_way(
    lhs.begin(), lhs.end(),
    rhs.begin(), rhs.end(),
    std::remove_cvref_t<decltype(lhs)>::node::cmp
  );
}

// iterators
iterator begin() noexcept
{
  return root_ ?
    iterator(&root_, detail::first_node(root_, {})) :
    iterator(&root_);
}

iterator end() noexcept { return iterator(&root_); }

// const iterators
const_iterator begin() const noexcept
{
  return root_ ?
    const_iterator(&root_, detail::first_node(root_, {})) :
    const_iterator(&root_);
}

const_iterator end() const noexcept { return const_iterator(&root_); }

const_iterator cbegin() const noexcept
{
  return root_ ?
    const_iterator(&root_, detail::first_node(root_, {})) :
    const_iterator(&root_);
}

const_iterator cend() const noexcept { return const_iterator(&root_); }

// reverse iterators
reverse_iterator rbegin() noexcept
{
  return reverse_iterator(iterator(&root_));
}

reverse_iterator rend() noexcept
{
  return root_ ?
    reverse_iterator(iterator(&root_, detail::first_node(root_, {}))) :
    reverse_iterator(iterator(&root_));
}

// const reverse iterators
const_reverse_iterator crbegin() const noexcept
{
  return const_reverse_iterator(const_iterator(&root_));
}

const_reverse_iterator crend() const noexcept
{
  return root_ ?
    const_reverse_iterator(
      const_iterator(&root_, detail::first_node(root_, {}))
    ) :
    const_reverse_iterator(const_iterator(&root_));
}

//
auto root() const noexcept { return root_; }

//
static constexpr size_type max_size() noexcept { return ~size_type{} / 3; }

void clear() noexcept(noexcept(delete root_))
{
  detail::destroy(root_, {});
  root_ = {};
}

bool empty() const noexcept { return !root_; }
void swap(this_class& o) noexcept { std::swap(root_, o.root_); }

//
bool contains(Key const& k) const noexcept
{
  return std::get<0>(detail::find(root_, {}, k));
}

//
iterator erase(const_iterator a, const_iterator const b)
  noexcept(noexcept(erase(a)))
{
  iterator i(b);

  for (; a != b; i = erase(a), a = i);

  return i;
}

//
iterator find(Key const& k) noexcept
{
  return {&root_, detail::find(root_, {}, k)};
}

const_iterator find(Key const& k) const noexcept
{
  return {&root_, detail::find(root_, {}, k)};
}

iterator find(auto const& k) noexcept
{
  return {&root_, detail::find(root_, {}, k)};
}

const_iterator find(auto const& k) const noexcept
{
  return {&root_, detail::find(root_, {}, k)};
}

// these may always throw
void insert(std::initializer_list<value_type> l)
  noexcept(noexcept(insert(l.begin(), l.end())))
  requires(std::is_copy_constructible_v<value_type>)
{
  insert(l.begin(), l.end());
}

//
iterator lower_bound(auto const& k) noexcept
{
  return std::get<0>(equal_range(k));
}

const_iterator lower_bound(auto const& k) const noexcept
{
  return std::get<0>(equal_range(k));
}

//
iterator upper_bound(auto const& k) noexcept
{
  return std::get<1>(equal_range(k));
}

const_iterator upper_bound(auto const& k) const noexcept
{
  return std::get<1>(equal_range(k));
}
