import csv


def get_countries():
    """Returns a list of countries."""
    with open('countries.csv', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        return [row for row in reader]


def main():
    """Main function."""
    countries = get_countries()
    print(countries)


if __name__ == '__main__':
    main()
