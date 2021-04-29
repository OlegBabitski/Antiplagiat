const getTemplate = (data = [], placeholder, selectedValue, img) => {
	let text = placeholder ?? 'Placeholder по умолчанию'

	const items = data.map(item => {
		let cls = ''
		if (item.id === selectedValue) {
			text = item.value
			cls = ' selected'
		}
		return `
			<li class="fb-select__item${cls}" data-value="${item.id}">${item.value}</li>
		`
	})

	return `
		<div class="fb-select__backdrop"></div>
		<div class="fb-select__input">
			<span class="fb-select__choice">${text}</span><img src="${img}" alt="Image" class="fb-select__img">
		</div>
		<div class="fb-select__dropdown">
			<ul class="fb-select__list">
				${items.join('')}
			</ul>
		</div>
	`
}

class fbSelect {
	constructor(selector, options) {
		this.$el = document.getElementById(selector)
		this.options = options
		this.selectedValue = options.selectedId
		this.img = options.img

		this.render()
		this.setup()
	}

	render() {
		this.$el.classList.add('fb-select')
		const {placeholder, classesToAdd, data} = this.options
		if(classesToAdd){
			for (var i = 0; i < classesToAdd.split(',').length; i++) {
				const classToAdd = classesToAdd.split(',')[i];
				this.$el.classList.add(classToAdd);
			}
		}

		this.$el.innerHTML = getTemplate(data, placeholder, this.selectedValue, this.img)
	}

	setup(){
		this.clickHandler = this.clickHandler.bind(this)
		this.$el.addEventListener('click', this.clickHandler)
		this.$choice = this.$el.querySelector('.fb-select__choice')
		this.$input = this.$el.querySelector('.fb-select__input')
	}

	clickHandler(event) {
		if (event.target.closest('.fb-select__item')){
			const value = event.target.dataset.value
			this.select(value)
		} else if (event.target.closest('.fb-select__input')) {
			this.toggle()
		} else {
			this.close()
		}
	}

	get isOpen() {
		return this.$el.classList.contains('open')
	}

	get current() {
		return this.options.data.find(item => item.id === this.selectedValue)
	}

	select(value) {
		this.selectedValue = value
		this.$choice.textContent = this.current.value

		this.$el.querySelectorAll('.fb-select__item').forEach(el => {
			el.classList.remove('selected')
		})
		this.$el.querySelector(`[data-value="${value}"]`).classList.add('selected')

		this.$input.style.backgroundImage = this.getBackgroundImageUrl(this.$el.querySelector('.selected'))

		this.options.onSelect ? this.options.onSelect(this.current) : null

		this.close()
	}

	toggle() {
		this.isOpen ? this.close() : this.open()
	}

	open() {
		this.$el.classList.add('open')
	}

	close() {
		this.$el.classList.remove('open')
	}

	destroy() {
		this.$el.removeEventListener('click', this.clickHandler)
	}

	getBackgroundImageUrl(item) {
		return getComputedStyle(item).backgroundImage;
	}
}