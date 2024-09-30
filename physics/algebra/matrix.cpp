#include "matrix.hpp"

using namespace GYDM;

/*************************************************************************************************/
GYDM::Matrix::~Matrix() noexcept {
    for (size_t r = 0; r < this->M; ++ r) {
        delete [] this->entries[r];
    }

    delete [] this->entries;
}

GYDM::Matrix::Matrix(size_t m, size_t n) noexcept : M(m), N(n) {
    this->entries = new Flonum*[this->M];

    for (size_t r = 0; r < this->M; ++ r) {
        this->entries[r] = new Flonum[this->N];
    }
}

GYDM::Matrix::Matrix(const GYDM::Matrix& lhs, const GYDM::Matrix& rhs, bool forward) noexcept {
    if (forward) {
        array2d_add(this->entries, lhs.entries, rhs.entries, this->M, this->N);
    } else {
        array2d_subtract(this->entries, lhs.entries, rhs.entries, this->M, this->N);
    }
}

GYDM::Matrix::Matrix(const GYDM::Matrix& lhs, Flonum rhs, bool forward) noexcept {
    if (forward) {
        array2d_scalar_multiply(this->entries, lhs.entries, rhs, this->M, this->N);
    } else {
        array2d_divide(this->entries, lhs.entries, rhs, this->M, this->N);
    }
}

GYDM::Matrix::Matrix(const GYDM::Matrix& src) noexcept : Matrix(src.M, src.N) {
    // this->fill(src);
}

GYDM::Matrix::Matrix(const GYDM::Matrix* src) noexcept : Matrix(src->M, src->N) {
    // this->fill(src);
}

/*************************************************************************************************/
Flonum GYDM::Matrix::ref(size_t r, size_t c) const {
    array2d_check_bounds(this->M, this->N, r, c);

    return this->entries[r][c];
}

void GYDM::Matrix::set(size_t r, size_t c, Flonum datum) {
    array2d_check_bounds(this->M, this->N, r, c);
    this->entries[r][c] = datum;
}

void GYDM::Matrix::swap_row(size_t r1, size_t r2) {
    if (array2d_rows_okay(this->M, r1, r2))
        array2d_swap_row(this->entries, this->N, r1, r2);
}

void GYDM::Matrix::swap_column(size_t c1, size_t c2) {
    if (array2d_columns_okay(this->N, c1, c2))
        array2d_swap_column(this->entries, this->M, c1, c2);
}

GYDM::Matrix& GYDM::Matrix::operator+=(const GYDM::Matrix& rhs) {
    array2d_add(this->entries, rhs.entries, this->M, this->N);
    
    return (*this);
}

GYDM::Matrix& GYDM::Matrix::operator-=(const GYDM::Matrix& rhs) {
    array2d_subtract(this->entries, rhs.entries, this->M, this->N);
    
    return (*this);
}
        
bool GYDM::Matrix::is_singular_matrix() const noexcept {
    return false;
}

bool GYDM::Matrix::is_scalar_matrix() const noexcept {
    return (this->M > 0)
        && this->is_diagonal_matrix()
        && array2d_diagonal_equal(this->entries, this->M, this->entries[0][0]);
}

bool GYDM::Matrix::LU_decomposite(GYDM::Matrix* L, GYDM::Matrix* U) const noexcept {
    // return array2d_lu_decomposite(this->entries, L->entries, U->entries);
    return false;
}

bool GYDM::Matrix::LUP_decomposite(GYDM::Matrix* L, GYDM::Matrix* U, GYDM::Matrix* P) const noexcept {
    // return array2d_lup_decomposite(this->entries, L->entries, U->entries, P->entries);
    return false;
}
