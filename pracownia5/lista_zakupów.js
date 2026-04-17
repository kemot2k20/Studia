/**
 * Lista produktów
 * @type {Array<{id: number, name: string, quantity: number, purchaseDate: Date, purchased: boolean, price: number | null}>}
 */
let products = [];
let nextId = 1;

/**
 * Dodaje nowy produkt do listy
 * @param {string} name - Nazwa produktu
 * @param {number} quantity - Liczba sztuk
 * @param {string} purchaseDateStr - Data zakupu w formacie RRRR-MM-DD
 */
function addProduct(name, quantity, purchaseDateStr) {
    const purchaseDate = new Date(purchaseDateStr);
    products.push({
        id: nextId++,
        name,
        quantity,
        purchaseDate,
        purchased: false,
        price: null
    });
}

/**
 * Usuwa produkt z listy
 * @param {number} id - ID produktu do usunięcia
 */
function deleteProduct(id) {
    products = products.filter(product => product.id !== id);
}

/**
 * Aktualizuje nazwę produktu
 * @param {number} id - ID produktu
 * @param {string} newName - Nowa nazwa
 */
function updateProductName(id, newName) {
    const product = products.find(p => p.id === id);
    if (product) product.name = newName;
}

/**
 * Aktualizuje status zakupu produktu
 * @param {number} id - ID produktu
 * @param {boolean} newStatus - Nowy status zakupu
 */
function updateProductStatus(id, newStatus) {
    const product = products.find(p => p.id === id);
    if (product) {
        product.purchased = newStatus;
        if (!newStatus) product.price = null;
    }
}

/**
 * Aktualizuje liczbę sztuk produktu
 * @param {number} id - ID produktu
 * @param {number} newQuantity - Nowa liczba sztuk
 */
function updateProductQuantity(id, newQuantity) {
    const product = products.find(p => p.id === id);
    if (product) product.quantity = newQuantity;
}

/**
 * Aktualizuje datę zakupu produktu
 * @param {number} id - ID produktu
 * @param {string} newDateStr - Nowa data w formacie RRRR-MM-DD
 */
function updateProductDate(id, newDateStr) {
    const product = products.find(p => p.id === id);
    if (product) product.purchaseDate = new Date(newDateStr);
}

/**
 * Zmienia kolejność produktów
 * @param {number} id - ID produktu
 * @param {number} newIndex - Nowa pozycja w liście
 */
function reorderProduct(id, newIndex) {
    const index = products.findIndex(p => p.id === id);
    if (index === -1) return;
    const [product] = products.splice(index, 1);
    products.splice(newIndex, 0, product);
}

/**
 * Zwraca produkty do zakupu dziś
 * @returns {Array} Lista produktów
 */
function getProductsDueToday() {
    const today = new Date();
    return products.filter(p => 
        !p.purchased && 
        p.purchaseDate.toDateString() === today.toDateString()
    );
}

/**
 * Ustawia cenę dla zakupionego produktu
 * @param {number} id - ID produktu
 * @param {number} price - Cena produktu
 */
function setProductPrice(id, price) {
    const product = products.find(p => p.id === id);
    if (product?.purchased) product.price = price;
}

/**
 * Oblicza koszt zakupów dla danego dnia
 * @param {string} dateStr - Data w formacie RRRR-MM-DD
 * @returns {number} Suma kosztów
 */
function calculateDailyCost(dateStr) {
    const targetDate = new Date(dateStr);
    return products
        .filter(p => 
            p.purchased && 
            p.price !== null && 
            p.purchaseDate.toDateString() === targetDate.toDateString()
        )
        .reduce((sum, p) => sum + (p.price * p.quantity), 0);
}

/**
 * Masowo modyfikuje produkty
 * @param {Array<number>} ids - Lista ID produktów
 * @param {Function} modifier - Funkcja modyfikująca (product) => void
 */
function bulkModifyProducts(ids, modifier) {
    products.forEach(p => {
        if (ids.includes(p.id)) modifier(p);
    });
}

// Inicjalizacja pustej listy
console.log("Początkowy stan listy:", products);

// 1. Dodawanie produktów
addProduct("Mleko", 2, "2024-05-20");
addProduct("Chleb", 1, "2024-05-21");
addProduct("Jabłka", 5, "2024-05-20");
console.log("Stan po dodaniu 3 produktów:", products);

/* Wynik:
3 obiekty z autogenerowanymi ID (1, 2, 3)
Wszystkie mają purchased: false i price: null
*/

// 2. Aktualizacja nazwy produktu 2
updateProductName(2, "Chleb pełnoziarnisty");
console.log("Po zmianie nazwy produktu 2:", products[1]);

// 3. Aktualizacja ilości produktu 1
updateProductQuantity(1, 3);
console.log("Po zmianie ilości produktu 1:", products[0]);

// 4. Oznaczenie produktu 3 jako zakupionego
updateProductStatus(3, true);
setProductPrice(3, 4.99); // Powinno zadziałać
setProductPrice(1, 2.50); // NIE zadziała (produkt niezakupiony)
console.log("Po zakupie jabłek:", products[2]);

// 5. Usuwanie produktu 2
deleteProduct(2);
console.log("Stan po usunięciu produktu 2:", products);

// 6. Zmiana kolejności - przenosimy jabłka na początek
reorderProduct(3, 0);
console.log("Po zmianie kolejności:", products.map(p => p.name));

// 7. Pobieranie produktów na dziś (2024-05-20)
console.log("Do kupienia dziś:", getProductsDueToday());

// 8. Obliczanie kosztów dla dnia
updateProductStatus(1, true); // Kupujemy mleko
setProductPrice(1, 3.20);
console.log("Koszt dzisiejszych zakupów:", calculateDailyCost("2024-05-20"));

// 9. Masowa modyfikacja - podwyżka cen o 10% dla wybranych
bulkModifyProducts([1, 3], product => {
    if (product.purchased && product.price) {
        product.price *= 1.10;
    }
});
console.log("Po masowej modyfikacji cen:", products);